#pragma once

#include <stdint.h>
#include <optional>

#include "geom.h"
#include "macros.h"

namespace ns {

enum class Component : uint8_t {
  kRed,
  kGreen,
  kBlue,
  kAlpha,
};

class Texture {
 public:
  Texture() = default;

  ~Texture() { std::free(allocation_); }

  Texture(Texture&& other) {
    std::swap(allocation_, other.allocation_);
    std::swap(size_, other.size_);
  }

  size_t GetBytesPerPixel() const { return sizeof(Color); }

  uint8_t* GetAllocation() { return allocation_; }

  const uint8_t* GetAllocation() const { return allocation_; }

  const uint8_t* GetComponent(Component comp, UPoint point) const {
    const uint8_t* comp_allocation =
        allocation_ + size_.GetArea() * static_cast<uint8_t>(comp);
    return (comp_allocation + size_.x * point.y) + point.x;
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

  void ToGrayscale();

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

  bool CopyRGBA(Texture& texture) const;

  void Composite(const Texture& texture, UPoint point);

  float AverageLuminance() const;

  void LuminanceThreshold(float luminance);

  static std::optional<Texture> CreateFromFile(const char* name);

 private:
  uint8_t* allocation_ = nullptr;
  UPoint size_ = {};

  NS_DISALLOW_COPY_AND_ASSIGN(Texture);
};

}  // namespace ns
