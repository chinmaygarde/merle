#include "texture.h"

#include <_types/_uint32_t.h>
#include <stdint.h>
#include <sys/_types/_int32_t.h>
#include "geom.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstring>
#include <optional>

#include "texture_ispc.h"

namespace ns {

std::optional<Texture> Texture::CreateFromFile(const char* name) {
  int x = 0;
  int y = 0;
  int channels = 0;

  stbi_uc* decoded = ::stbi_load(name, &x, &y, &channels, STBI_rgb_alpha);

  if (decoded == nullptr || x < 0 || y < 0) {
    std::cout << "Could not load image: " << name << std::endl;
    return std::nullopt;
  }

  Texture texture;

  if (!texture.Resize({static_cast<uint32_t>(x), static_cast<uint32_t>(y)})) {
    ::stbi_image_free(decoded);
    std::cout << "Could not create texture allocation for image: " << name
              << std::endl;
    return std::nullopt;
  }

  ispc::FromRGBA(reinterpret_cast<ispc::Color*>(decoded),  // rgba
                 texture.GetRedMutable(),                  // red
                 texture.GetGreenMutable(),                // green
                 texture.GetBlueMutable(),                 // blue
                 texture.GetAlphaMutable(),                // alpha
                 texture.GetPixelCount()                   // length
  );

  ::stbi_image_free(decoded);

  return texture;
}

void Texture::Clear(Color color,
                    bool clear_red,
                    bool clear_green,
                    bool clear_blue,
                    bool clear_alpha) {
  const auto length = size_.x * size_.y;
  if (clear_red) {
    ::memset(GetRedMutable(), color.red, length);
  }
  if (clear_green) {
    ::memset(GetGreenMutable(), color.green, length);
  }
  if (clear_blue) {
    ::memset(GetBlueMutable(), color.blue, length);
  }
  if (clear_alpha) {
    ::memset(GetAlphaMutable(), color.alpha, length);
  }
}

void Texture::PremultiplyAlpha() {
  const auto length = size_.x * size_.y;
  ispc::PremultiplyAlpha(GetRedMutable(),    // r
                         GetGreenMutable(),  // g
                         GetBlueMutable(),   // b
                         GetAlphaMutable(),  // a
                         length              // length
  );
}

void Texture::Grayscale() {
  const auto length = size_.GetArea();
  ispc::Grayscale(GetRedMutable(),    // red
                  GetGreenMutable(),  // green
                  GetBlueMutable(),   // blue
                  length              // length
  );
}

void Texture::Replace(const Texture& texture, Point offset) {
  Rect src_rect(offset, Size{static_cast<int32_t>(texture.GetSize().x),
                             static_cast<int32_t>(texture.GetSize().y)});
  const auto& dst_rect =
      Rect{Size{static_cast<int32_t>(size_.x), static_cast<int32_t>(size_.y)}}
          .Intersection(src_rect);

  if (!dst_rect.has_value()) {
    return;
  }

  for (auto y = 0; y < dst_rect->size.y; y++) {
    const auto src_point = UPoint{static_cast<uint32_t>(offset.x),
                                  static_cast<uint32_t>(offset.y + y)};
    const auto dst_point = UPoint(0, static_cast<uint32_t>(y));
    ::memcpy(GetAllocationMutable(Component::kRed, src_point),   //
             texture.GetAllocation(Component::kRed, dst_point),  //
             dst_rect->size.x);
    ::memcpy(GetAllocationMutable(Component::kGreen, src_point),   //
             texture.GetAllocation(Component::kGreen, dst_point),  //
             dst_rect->size.x);
    ::memcpy(GetAllocationMutable(Component::kBlue, src_point),   //
             texture.GetAllocation(Component::kBlue, dst_point),  //
             dst_rect->size.x);
    ::memcpy(GetAllocationMutable(Component::kAlpha, src_point),   //
             texture.GetAllocation(Component::kAlpha, dst_point),  //
             dst_rect->size.x);
  }
}

bool Texture::CopyToRGBA(Texture& texture) const {
  if (texture.GetSize() != GetSize()) {
    return false;
  }
  const auto length = size_.x * size_.y;
  ispc::CopyToRGBA(GetRed(),    // red
                   GetGreen(),  // green
                   GetBlue(),   // blue
                   GetAlpha(),  // alpha
                   reinterpret_cast<ispc::Color*>(
                       texture.GetAllocationMutable()),  // color(out)
                   length                                // length
  );
  return true;
}

void Texture::Invert() {
  const auto length = size_.x * size_.y;
  ispc::Invert(GetRedMutable(),    // red
               GetGreenMutable(),  // green
               GetBlueMutable(),   // blue
               length              // length
  );
}

void Texture::Exposure(float exposure) {
  const auto length = size_.x * size_.y;
  ispc::Exposure(GetRedMutable(),    // red
                 GetGreenMutable(),  // green
                 GetBlueMutable(),   // blue
                 exposure,           // exposure
                 length              // length
  );
}

void Texture::Brightness(float brightness) {
  const auto length = size_.x * size_.y;
  ispc::Brightness(GetRedMutable(),    // red
                   GetGreenMutable(),  // green
                   GetBlueMutable(),   // blue
                   brightness,         // brightness
                   length              // length
  );
}

void Texture::RGBALevels(float red, float green, float blue, float alpha) {
  const auto length = size_.x * size_.y;
  ispc::RGBALevels(GetRedMutable(),    // red
                   GetGreenMutable(),  // green
                   GetBlueMutable(),   // blue
                   GetAlphaMutable(),  // alpha
                   red,                // red level
                   green,              // green level
                   blue,               // blue level
                   alpha,              // alpha level
                   length              // length
  );
}

void Texture::Swizzle(Component red,
                      Component green,
                      Component blue,
                      Component alpha) {
  const auto length = size_.x * size_.y;
  ispc::Swizzle(GetRedMutable(),                      // red
                GetGreenMutable(),                    // green
                GetBlueMutable(),                     // blue
                GetAlphaMutable(),                    // alpha
                static_cast<ispc::Component>(red),    // red swizzle
                static_cast<ispc::Component>(green),  // green swizzle
                static_cast<ispc::Component>(blue),   // blue swizzle
                static_cast<ispc::Component>(alpha),  // alpha swizzle
                length                                // length
  );
}

void Texture::ColorMatrix(const Matrix& matrix) {
  const auto length = size_.x * size_.y;
  ispc::ColorMatrix(GetRedMutable(),    // red
                    GetGreenMutable(),  // green
                    GetBlueMutable(),   // blue
                    GetAlphaMutable(),  // alpha
                    length,             // length
                    reinterpret_cast<const ispc::Matrix&>(matrix.e));
}

void Texture::Sepia() {
  ColorMatrix(Matrix{
      0.3588, 0.7044, 0.1368, 0.0,  //
      0.2990, 0.5870, 0.1140, 0.0,  //
      0.2392, 0.4696, 0.0912, 0.0,  //
      0, 0, 0, 1.0,                 //
  });
}

void Texture::Contrast(float contrast) {
  const auto length = size_.x * size_.y;
  ispc::Contrast(GetRedMutable(),    // red
                 GetGreenMutable(),  // green
                 GetBlueMutable(),   // blue
                 length,             // length
                 contrast            // contrast
  );
}

void Texture::Saturation(float saturation) {
  const auto length = size_.x * size_.y;
  ispc::Saturation(GetRedMutable(),    // red
                   GetGreenMutable(),  // green
                   GetBlueMutable(),   // blue
                   length,             // length
                   saturation          // saturation
  );
}

void Texture::Vibrance(float vibrance) {
  const auto length = size_.x * size_.y;
  ispc::Saturation(GetRedMutable(),    // red
                   GetGreenMutable(),  // green
                   GetBlueMutable(),   // blue
                   length,             // length
                   vibrance            // vibrance
  );
}

void Texture::Hue(Radians hue) {
  const auto length = size_.x * size_.y;
  ispc::Hue(GetRedMutable(),    // red
            GetGreenMutable(),  // green
            GetBlueMutable(),   // blue
            length,             // length
            hue.radians         // hue
  );
}

void Texture::Opacity(UnitScalarF opacity) {
  const auto length = size_.x * size_.y;
  ispc::Opacity(GetAlphaMutable(),  // alphas
                length,             // length
                opacity             // opacity
  );
}

float Texture::AverageLuminance() const {
  const auto length = size_.x * size_.y;
  return ispc::AverageLuminance(GetRed(),    // red
                                GetGreen(),  // green
                                GetBlue(),   // blue
                                length       // length
  );
}

void Texture::LuminanceThreshold(float luminance) {
  const auto length = size_.x * size_.y;
  ispc::LuminanceThreshold(GetRedMutable(),    // red
                           GetGreenMutable(),  // green
                           GetBlueMutable(),   // blue
                           length,             // length
                           luminance           // luma threshold
  );
}

bool Texture::BoxBlur(const Texture& src, uint8_t radius) {
  const auto kernel_width = 2 * radius + 1;
  const auto kernel_length = kernel_width * kernel_width;
  std::vector<float> kernel(kernel_length, 1.0f / kernel_length);
  return ConvolutionNxN(src, kernel);
}

static std::vector<float> CreateGaussianKernel(uint8_t radius, float sigma) {
  std::vector<float> kernel;
  size_t kernel_width = 2 * radius + 1;
  kernel.resize(kernel_width * kernel_width);

  float gaussian_sum = 0.0f;
  // Construct the Gaussian kernel.
  for (int8_t y = -radius; y < radius + 1; y++) {
    for (int8_t x = -radius; x < radius + 1; x++) {
      // https://en.wikipedia.org/wiki/Gaussian_blur
      float gauss = std::exp(-((x * x) + (y * y)) / (2 * sigma * sigma)) /
                    (2 * kPi * sigma * sigma);

      kernel[(y + radius) * kernel_width + (x + radius)] = gauss;
      gaussian_sum += gauss;
    }
  }
  // Normalize the kernel.
  for (int8_t y = -radius; y < radius + 1; y++) {
    for (int8_t x = -radius; x < radius + 1; x++) {
      kernel[(y + radius) * kernel_width + (x + radius)] /= gaussian_sum;
    }
  }
  return kernel;
}

bool Texture::GaussianBlur(const Texture& src, uint8_t radius, float sigma) {
  return ConvolutionNxN(src, CreateGaussianKernel(radius, sigma));
}

bool Texture::Sobel(const Texture& src,
                    Component src_component,
                    Component dst_component) {
  if (size_ != src.size_) {
    return false;
  }
  const auto length = size_.x * size_.y;
  ispc::Sobel(
      src.allocation_ + length * static_cast<uint8_t>(src_component),  // src
      allocation_ + length * static_cast<uint8_t>(dst_component),      // dst
      size_.x,                                                         // width
      size_.y                                                          // height
  );
  return true;
}

void Texture::DuplicateChannel(Component src, Component dst) {
  if (src == dst) {
    return;
  }

  const auto length = size_.x * size_.y;
  ::memmove(allocation_ + length * static_cast<uint8_t>(dst),  // dst
            allocation_ + length * static_cast<uint8_t>(src),  // src
            length                                             // length
  );
}

bool Texture::ConvolutionNxN(const Texture& src,
                             const std::vector<float>& kernel) {
  if (size_ != src.size_) {
    return false;
  }
  ispc::ConvolutionNxN(src.GetRed(),                       // src r
                       src.GetGreen(),                     // src g
                       src.GetBlue(),                      // src b
                       src.GetAlpha(),                     // src a
                       GetRedMutable(),                    // dst r
                       GetGreenMutable(),                  // dst g
                       GetBlueMutable(),                   // dst b
                       GetAlphaMutable(),                  // dst a
                       size_.x,                            // width
                       size_.y,                            // height
                       const_cast<float*>(kernel.data()),  // kernel
                       kernel.size()                       // kernel size
  );
  return true;
}

bool Texture::FadeTransition(const Texture& from,
                             const Texture& to,
                             UnitScalarF t) {
  if (from.size_ != to.size_) {
    return false;
  }
  const auto length = size_.x * size_.y;
  ispc::FadeTransition(GetRedMutable(),    // dst_r
                       GetGreenMutable(),  // dst_g
                       GetBlueMutable(),   // dst_b
                       GetAlphaMutable(),  // dst_a
                       from.GetRed(),      // from_r
                       from.GetGreen(),    // from_g
                       from.GetBlue(),     // from_b
                       from.GetAlpha(),    // from_a
                       to.GetRed(),        // to_r
                       to.GetGreen(),      // to_g
                       to.GetBlue(),       // to_b
                       to.GetAlpha(),      // to_a
                       length,             // len
                       t                   // t
  );
  return true;
}

bool Texture::SwipeTransition(const Texture& from,
                              const Texture& to,
                              UnitScalarF t,
                              Direction direction) {
  if (from.size_ != to.size_) {
    return false;
  }

  switch (direction) {
    case Direction::kHorizontal:
      ispc::SwipeTransitionHorizontal(GetRedMutable(),    // dst_r
                                      GetGreenMutable(),  // dst_g
                                      GetBlueMutable(),   // dst_b
                                      GetAlphaMutable(),  // dst_a
                                      from.GetRed(),      // from_r
                                      from.GetGreen(),    // from_g
                                      from.GetBlue(),     // from_b
                                      from.GetAlpha(),    // from_a
                                      to.GetRed(),        // to_r
                                      to.GetGreen(),      // to_g
                                      to.GetBlue(),       // to_b
                                      to.GetAlpha(),      // to_a
                                      size_.x,            // width
                                      size_.y,            // height
                                      t                   // t
      );
      break;
    case Direction::kVertical:
      ispc::SwipeTransitionVertical(GetRedMutable(),    // dst_r
                                    GetGreenMutable(),  // dst_g
                                    GetBlueMutable(),   // dst_b
                                    GetAlphaMutable(),  // dst_a
                                    from.GetRed(),      // from_r
                                    from.GetGreen(),    // from_g
                                    from.GetBlue(),     // from_b
                                    from.GetAlpha(),    // from_a
                                    to.GetRed(),        // to_r
                                    to.GetGreen(),      // to_g
                                    to.GetBlue(),       // to_b
                                    to.GetAlpha(),      // to_a
                                    size_.x,            // width
                                    size_.y,            // height
                                    t                   // t
      );
      break;
  }

  return true;
}

Color Texture::AverageColor() const {
  const auto length = size_.x * size_.y;
  ispc::Color color = {};
  ispc::AverageColor(GetRed(),    // dst_r
                     GetGreen(),  // dst_g
                     GetBlue(),   // dst_b
                     GetAlpha(),  // dst_a
                     length, color);
  return Color{color.red, color.green, color.blue, color.alpha};
}

bool Texture::IsOpaque() const {
  const auto length = size_.x * size_.y;
  return ispc::AllEqual(GetAlpha(),  // dst_a
                        length,      // length
                        255          // value
  );
}

}  // namespace ns
