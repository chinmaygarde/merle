#pragma once

#include <SDL3/SDL.h>
#include "macros.h"

namespace ns {

struct SDLTextureNoCopyCaster {
  SDLTextureNoCopyCaster(SDL_Renderer* renderer,
                         const void* pixels,
                         int width,
                         int height,
                         int bytes_per_pixel);

  ~SDLTextureNoCopyCaster();

  operator bool() const;

  operator SDL_Texture*() const;

 private:
  SDL_Surface* surface_ = nullptr;
  SDL_Texture* texture_ = nullptr;

  NS_DISALLOW_COPY_AND_ASSIGN(SDLTextureNoCopyCaster);
};

}  // namespace ns
