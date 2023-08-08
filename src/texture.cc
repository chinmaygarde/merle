#include "texture.h"

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

  auto allocation = texture.GetAllocation();
  auto length = texture.GetSize().GetArea();

  ispc::FromRGBA(reinterpret_cast<ispc::Color*>(decoded),  // rgba
                 allocation + length * 0,                  // red
                 allocation + length * 1,                  // green
                 allocation + length * 2,                  // blue
                 allocation + length * 3,                  // alpha
                 length                                    // length
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
    ::memset(allocation_ + length * 0, color.red, length);
  }
  if (clear_green) {
    ::memset(allocation_ + length * 1, color.green, length);
  }
  if (clear_blue) {
    ::memset(allocation_ + length * 2, color.blue, length);
  }
  if (clear_alpha) {
    ::memset(allocation_ + length * 3, color.alpha, length);
  }
}

void Texture::ToGrayscale() {
  const auto length = size_.GetArea();
  ispc::ToGrayscale(allocation_ + length * 0,  // red
                    allocation_ + length * 1,  // green
                    allocation_ + length * 2,  // blue
                    length                     // length
  );
}

void Texture::Composite(const Texture& texture, UPoint point) {
  Rect texture_rect(point, texture.GetSize());
  const auto& rect = Rect{size_}.Intersection(texture_rect);
  if (!rect.has_value()) {
    return;
  }

  for (uint32_t y = 0; y < rect->size.y; y++) {
    ::memcpy(const_cast<uint8_t*>(
                 GetComponent(Component::kRed, {point.x, point.y + y})),  //
             texture.GetComponent(Component::kRed, {0, y}),               //
             rect->size.x);
    ::memcpy(const_cast<uint8_t*>(
                 GetComponent(Component::kGreen, {point.x, point.y + y})),  //
             texture.GetComponent(Component::kGreen, {0, y}),               //
             rect->size.x);
    ::memcpy(const_cast<uint8_t*>(
                 GetComponent(Component::kBlue, {point.x, point.y + y})),  //
             texture.GetComponent(Component::kBlue, {0, y}),               //
             rect->size.x);
    ::memcpy(const_cast<uint8_t*>(
                 GetComponent(Component::kAlpha, {point.x, point.y + y})),  //
             texture.GetComponent(Component::kAlpha, {0, y}),               //
             rect->size.x);
  }
}

bool Texture::CopyRGBA(Texture& texture) const {
  if (texture.GetSize() != GetSize()) {
    return false;
  }
  const auto length = size_.x * size_.y;
  ispc::ToRGBA(
      allocation_ + length * 0,                                 // red
      allocation_ + length * 1,                                 // green
      allocation_ + length * 2,                                 // blue
      allocation_ + length * 3,                                 // alpha
      reinterpret_cast<ispc::Color*>(texture.GetAllocation()),  // color(out)
      length                                                    // length
  );
  return true;
}

void Texture::Invert() {
  const auto length = size_.x * size_.y;
  ispc::Invert(allocation_ + length * 0,  // red
               allocation_ + length * 1,  // green
               allocation_ + length * 2,  // blue
               length                     // length
  );
}

void Texture::Exposure(float exposure) {
  const auto length = size_.x * size_.y;
  ispc::Exposure(allocation_ + length * 0,  // red
                 allocation_ + length * 1,  // green
                 allocation_ + length * 2,  // blue
                 exposure,                  // exposure
                 length                     // length
  );
}

void Texture::Brightness(float brightness) {
  const auto length = size_.x * size_.y;
  ispc::Brightness(allocation_ + length * 0,  // red
                   allocation_ + length * 1,  // green
                   allocation_ + length * 2,  // blue
                   brightness,                // brightness
                   length                     // length
  );
}

void Texture::RGBALevels(float red, float green, float blue, float alpha) {
  const auto length = size_.x * size_.y;
  ispc::RGBALevels(allocation_ + length * 0,  // red
                   allocation_ + length * 1,  // green
                   allocation_ + length * 2,  // blue
                   allocation_ + length * 3,  // alpha
                   red,                       // red level
                   green,                     // green level
                   blue,                      // blue level
                   alpha,                     // alpha level
                   length                     // length
  );
}

void Texture::Swizzle(Component red,
                      Component green,
                      Component blue,
                      Component alpha) {
  const auto length = size_.x * size_.y;
  ispc::Swizzle(allocation_ + length * 0,             // red
                allocation_ + length * 1,             // green
                allocation_ + length * 2,             // blue
                allocation_ + length * 3,             // alpha
                static_cast<ispc::Component>(red),    // red swizzle
                static_cast<ispc::Component>(green),  // green swizzle
                static_cast<ispc::Component>(blue),   // blue swizzle
                static_cast<ispc::Component>(alpha),  // alpha swizzle
                length                                // length
  );
}

}  // namespace ns
