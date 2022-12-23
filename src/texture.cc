#include "texture.h"

#include "simd/register.h"

namespace ns {

static bool gHaveNeon = true;
void SetHaveNeon(bool have_neon) {
  gHaveNeon = have_neon;
}

void Texture::Clear(Color color) {
  if (!gHaveNeon) {
    auto* colors = reinterpret_cast<uint32_t*>(allocation_);
    for (size_t i = 0; i < size_.x * size_.y; i++) {
      colors[i] = color.color;
    }
    return;
  }
  // This is about as fast as:
  // memset_pattern4(allocation_, &color, size_.x * size_.y * sizeof(Color));

  auto colors = reinterpret_cast<uint32_t*>(allocation_);
  const auto size = size_.x * size_.y;
  const auto rem = size % 4;
  RegisterUInt32x4 color_simd(color.color);
  for (size_t i = 0; i < size - rem; i += 4) {
    color_simd.Store(colors + i);
  }
  for (size_t i = size - rem; i < size; i++) {
    colors[i] = color.color;
  }
}

void Texture::ConvertToGrayScale() {
  if (!gHaveNeon) {
    auto* colors = reinterpret_cast<Color*>(allocation_);
    for (size_t i = 0; i < size_.x * size_.y; i++) {
      Color color = colors[i];
      auto average = (color.GetRed() + color.GetGreen() + color.GetBlue()) / 3;
      colors[i] =
          Color{static_cast<uint8_t>(average), static_cast<uint8_t>(average),
                static_cast<uint8_t>(average), 255};
    }
    return;
  }

  for (size_t i = 0, pixels = size_.x * size_.y; i < pixels; i += 4) {
  }
}

}  // namespace ns
