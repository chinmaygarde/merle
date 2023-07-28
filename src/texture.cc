#include "texture.h"

#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <arm_neon.h>
#include <cstring>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ns {

std::optional<Texture> Texture::CreateFromFile(const char* name) {
  int x = 0;
  int y = 0;
  int channels = 0;

  stbi_uc* decoded = ::stbi_load(name, &x, &y, &channels, STBI_rgb_alpha);

  if (channels != STBI_rgb_alpha || x < 0 || y < 0) {
    return std::nullopt;
  }

  Texture texture;

  if (!texture.Resize({static_cast<uint32_t>(x), static_cast<uint32_t>(y)})) {
    ::stbi_image_free(decoded);
    return std::nullopt;
  }

  ::memmove(reinterpret_cast<uint8_t*>(texture.At()),  //
            decoded,                                   //
            x * y * STBI_rgb_alpha                     //
  );

  ::stbi_image_free(decoded);

  return texture;
}

void Texture::Clear(Color color) {
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
  auto* colors = reinterpret_cast<Color*>(allocation_);
  for (size_t i = 0; i < size_.x * size_.y; i++) {
    auto old = colors[i];
    auto level = static_cast<uint8_t>((old.red + old.green + old.blue) / 3);
    colors[i] = Color(level, level, level, old.alpha);
  }
}

}  // namespace ns
