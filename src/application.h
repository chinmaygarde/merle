#pragma once

#include <SDL3/SDL.h>

#include <memory>

#include "geom.h"
#include "macros.h"
#include "sdl_utils.h"

namespace ns {

class Rasterizer;

class Application {
 public:
  Application(UPoint size = {800, 600});

  ~Application();

  bool IsValid() const;

  bool Render();

  bool Update();

  bool OnWindowSizeChanged(UPoint size);

 private:
  UPoint window_size_;
  SDL_Window* sdl_window_ = nullptr;
  SDL_Renderer* sdl_renderer_ = nullptr;
  bool is_valid_ = false;

  bool OnRender();

  NS_DISALLOW_COPY_AND_ASSIGN(Application);
};

}  // namespace ns
