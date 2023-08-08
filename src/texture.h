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

  bool CopyRGBA(Texture& texture) const;

  void Composite(const Texture& texture, UPoint point);

  static std::optional<Texture> CreateFromFile(const char* name);

 private:
  uint8_t* allocation_ = nullptr;
  UPoint size_ = {};

  NS_DISALLOW_COPY_AND_ASSIGN(Texture);
};

}  // namespace ns
