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
  ispc::Clear(reinterpret_cast<uint32_t*>(allocation_),  //
              size_.x * size_.y,                         //
              color.color                                //
  );
}

void Texture::ToGrayscale() {
  ispc::ToGrayscale(reinterpret_cast<ispc::Color*>(allocation_),  //
                    size_.x * size_.y                             //
  );
}

}  // namespace ns
