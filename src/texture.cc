#include "texture.h"

#include <arm_neon.h>

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
  auto color_simd = vdupq_n_u32(color.color);
  for (size_t i = 0; i < size - rem; i += 4) {
    vst1q_u32(colors + i, color_simd);
  }
  for (size_t i = size - rem; i < size; i++) {
    colors[i] = color.color;
  }
}

void Texture::ToGrayscale() {
  if (!gHaveNeon) {
    auto* colors = reinterpret_cast<Color*>(allocation_);
    for (size_t i = 0; i < size_.x * size_.y; i++) {
      auto old = colors[i];
      auto level = static_cast<uint8_t>(
          (old.GetRed() + old.GetGreen() + old.GetBlue()) / 3);
      colors[i] = Color(level, level, level, old.GetAlpha());
    }
    return;
  }
  for (size_t i = 0; i < size_.x * size_.y; i++) {
    uint8x8x4_t colors = vld4_u8((uint8_t*)allocation_);
  }
}

}  // namespace ns
