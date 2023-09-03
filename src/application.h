#pragma once

#include <SDL3/SDL.h>

#include <functional>
#include <memory>

#include "geom.h"
#include "macros.h"
#include "sdl_utils.h"
#include "texture.h"

namespace merle {

class Rasterizer;

class Application {
 public:
  Application(UPoint size = {800, 600});

  ~Application();

  bool IsValid() const;

  bool Render();

  bool OnWindowSizeChanged(UPoint size);

  UPoint GetWindowSize() const;

  using ApplicationCallback =
      std::function<std::shared_ptr<Texture>(const Application& application)>;
  void SetRasterizerCallback(ApplicationCallback callback);

 private:
  UPoint window_size_;
  SDL_Window* sdl_window_ = nullptr;
  SDL_Renderer* sdl_renderer_ = nullptr;
  ApplicationCallback application_callback_;
  Texture onscreen_tex_;
  bool is_valid_ = false;

  bool OnRender();

  MERLE_DISALLOW_COPY_AND_ASSIGN(Application);
};

}  // namespace merle
