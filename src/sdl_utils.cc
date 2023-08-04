#include "sdl_utils.h"

namespace ns {

SDLTextureNoCopyCaster::SDLTextureNoCopyCaster(SDL_Renderer* renderer,
                                               const void* pixels,
                                               int width,
                                               int height,
                                               int bytes_per_pixel) {
  if (pixels == nullptr) {
    return;
  }
  auto surface = ::SDL_CreateSurfaceFrom(const_cast<void*>(pixels),  // pixels
                                         width,                      // width
                                         height,                     // height
                                         width * bytes_per_pixel,    // pitch
                                         SDL_PIXELFORMAT_ABGR8888    // format
  );
  if (surface == NULL) {
    std::cerr << "Could not create surface: " << SDL_GetError() << std::endl;
    return;
  }
  surface_ = surface;
  auto texture = ::SDL_CreateTextureFromSurface(renderer, surface_);
  if (texture == NULL) {
    std::cerr << "Could not create texture: " << SDL_GetError() << std::endl;
    return;
  }
  texture_ = texture;
}

SDLTextureNoCopyCaster::~SDLTextureNoCopyCaster() {
  if (texture_) {
    ::SDL_DestroyTexture(texture_);
  }

  if (surface_) {
    ::SDL_FreeSurface(surface_);
  }
}

SDLTextureNoCopyCaster::operator bool() const {
  return static_cast<SDL_Texture*>(*this) != NULL;
}

SDLTextureNoCopyCaster::operator SDL_Texture*() const {
  return texture_;
}

}  // namespace ns
