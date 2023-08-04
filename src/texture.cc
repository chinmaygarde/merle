#include "texture.h"

#include <cstring>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

  ::memcpy(reinterpret_cast<uint8_t*>(texture.At()),  //
           decoded,                                   //
           x * y * STBI_rgb_alpha                     //
  );

  ::stbi_image_free(decoded);

  return texture;
}

void Texture::Clear(Color color) {
  ispc::Clear(reinterpret_cast<uint32_t*>(allocation_),  //
              size_.x * size_.y,                         //
              color.color                                //
  );
}

void Texture::ToGrayscale() {
  ispc::ToGrayscale(reinterpret_cast<uint32_t*>(allocation_),  //
                    size_.x * size_.y                          //
  );
}

void Texture::Composite(const Texture& texture, UPoint point) {
  Rect texture_rect(point, texture.GetSize());
  const auto& rect = Rect{size_}.Intersection(texture_rect);
  if (!rect.has_value()) {
    return;
  }
  for (uint32_t y = 0; y < rect->size.y; y++) {
    const uint8_t* src = reinterpret_cast<const uint8_t*>(texture.At({0, y}));
    uint8_t* dst = reinterpret_cast<uint8_t*>(At({point.x, point.y + y}));
    ::memcpy(dst, src, rect->size.x * 4);
  }
}

}  // namespace ns
