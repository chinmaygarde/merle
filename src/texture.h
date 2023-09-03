#pragma once

#include <_types/_uint8_t.h>
#include <stdint.h>
#include <optional>
#include <vector>

#include "geom.h"
#include "macros.h"

namespace merle {

enum class Component : uint8_t {
  kRed,
  kGreen,
  kBlue,
  kAlpha,
};

class Texture {
 public:
  static std::optional<Texture> CreateFromFile(const char* name);

  Texture() = default;

  ~Texture() { std::free(allocation_); }

  Texture(Texture&& other) {
    std::swap(allocation_, other.allocation_);
    std::swap(size_, other.size_);
  }

  size_t GetBytesPerPixel() const { return sizeof(Color); }

  size_t GetPixelCount() const { return size_.GetArea(); }

  const uint8_t* GetAllocation(Component comp = Component::kRed,
                               UPoint point = {}) const {
    const uint8_t* allocation =
        allocation_ + size_.GetArea() * static_cast<uint8_t>(comp);
    return (allocation + size_.x * point.y) + point.x;
  }

  uint8_t* GetAllocationMutable(Component comp = Component::kRed,
                                UPoint point = {}) {
    return const_cast<uint8_t*>(GetAllocation(comp, point));
  }

  const uint8_t* GetRed(UPoint point = {}) const {
    return GetAllocation(Component::kRed, point);
  }

  const uint8_t* GetGreen(UPoint point = {}) const {
    return GetAllocation(Component::kGreen, point);
  }

  const uint8_t* GetBlue(UPoint point = {}) const {
    return GetAllocation(Component::kBlue, point);
  }

  const uint8_t* GetAlpha(UPoint point = {}) const {
    return GetAllocation(Component::kAlpha, point);
  }

  auto GetRedMutable(UPoint point = {}) {
    return GetAllocationMutable(Component::kRed, point);
  }

  auto GetGreenMutable(UPoint point = {}) {
    return GetAllocationMutable(Component::kGreen, point);
  }

  auto GetBlueMutable(UPoint point = {}) {
    return GetAllocationMutable(Component::kBlue, point);
  }

  auto GetAlphaMutable(UPoint point = {}) {
    return GetAllocationMutable(Component::kAlpha, point);
  }

  bool Resize(UPoint size) {
    if (size_ == size) {
      return true;
    }
    const auto new_allocation_size = size.x * size.y * GetBytesPerPixel();
    auto new_allocation = std::realloc(allocation_, new_allocation_size);
    if (new_allocation == nullptr) {
      return false;
    }
    allocation_ = reinterpret_cast<uint8_t*>(new_allocation);
    size_ = size;
    return true;
  }

  const UPoint& GetSize() const { return size_; }

  void Clear(Color color,
             bool clear_red = true,
             bool clear_green = true,
             bool clear_blue = true,
             bool clear_alpha = true);

  void PremultiplyAlpha();

  void Grayscale();

  void Invert();

  void Exposure(float exposure);

  void Brightness(float brightness);

  void RGBALevels(float red, float green, float blue, float alpha);

  void Swizzle(Component red, Component green, Component blue, Component alpha);

  void ColorMatrix(const Matrix& matrix);

  void Sepia();

  void Contrast(float contrast);

  //----------------------------------------------------------------------------
  /// @brief      Adjust the saturation of the image. Negative values desaturate
  ///             the image and positive values increase the saturation. The
  ///             values must be between -1.0f and 1.0f.
  ///
  /// @param[in]  saturation  The saturation
  ///
  void Saturation(float saturation = 0.0f);

  //----------------------------------------------------------------------------
  /// @brief      Adjust the vibrance of the image. The values must be between
  ///            -2.0f and 2.0f.
  ///
  /// @param[in]  vibrance  The vibrance
  ///
  void Vibrance(float vibrance = 0.0f);

  void Hue(Radians hue);

  void Opacity(UnitScalarF opacity);

  bool CopyToRGBA(Texture& texture) const;

  void Replace(const Texture& texture, Point point);

  float AverageLuminance() const;

  void LuminanceThreshold(float luminance);

  bool BoxBlur(const Texture& src, uint8_t radius = 1u);

  bool GaussianBlur(const Texture& src, uint8_t radius, float sigma);

  bool ConvolutionNxN(const Texture& src, const std::vector<float>& kernel);

  bool Sobel(const Texture& src,
             Component src_component,
             Component dst_component);

  void DuplicateChannel(Component src, Component dst);

  bool FadeTransition(const Texture& from, const Texture& to, UnitScalarF t);

  Color AverageColor() const;

  bool IsOpaque() const;

  enum class Direction {
    kHorizontal,
    kVertical,
  };
  bool SwipeTransition(const Texture& from,
                       const Texture& to,
                       UnitScalarF t,
                       Direction direction);

 private:
  uint8_t* allocation_ = nullptr;
  UPoint size_ = {};

  NS_DISALLOW_COPY_AND_ASSIGN(Texture);
};

}  // namespace merle
