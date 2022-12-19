#include "application.h"

namespace ns {

Application::Application(UPoint size) {
  window_size_ = size;

  Uint32 window_flags = 0;

  // window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
  window_flags |= SDL_WINDOW_RESIZABLE;

  sdl_window_ = ::SDL_CreateWindow("Neon Sandbox",
                                   SDL_WINDOWPOS_CENTERED,  //
                                   SDL_WINDOWPOS_CENTERED,  //
                                   window_size_.x,          //
                                   window_size_.y,          //
                                   window_flags             //
  );

  if (!sdl_window_) {
    std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
    return;
  }

  sdl_renderer_ = ::SDL_CreateRenderer(sdl_window_, nullptr, 0);

  if (!sdl_renderer_) {
    std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
    return;
  }

  is_valid_ = true;
}

Application::~Application() {
  if (sdl_window_) {
    ::SDL_DestroyWindow(sdl_window_);
  }
  if (sdl_renderer_) {
    ::SDL_DestroyRenderer(sdl_renderer_);
  }
}

bool Application::Render() {
  return OnRender();
}

bool Application::IsValid() const {
  return is_valid_;
}

bool Application::Update() {
  return true;
}

bool Application::OnRender() {
  if (!is_valid_) {
    std::cerr << "Application was invalid." << std::endl;
    return false;
  }

  if (!Update()) {
    return false;
  }

  // SDLTextureNoCopyCaster color_attachment(sdl_renderer_,               //
  //                                         texture->Get({}, 0),         //
  //                                         size.x,                      //
  //                                         size.y,                      //
  //                                         texture->GetBytesPerPixel()  //
  // );

  // SDL_Rect dest = {};
  // dest.x = 0;
  // dest.y = 0;
  // dest.w = size.x;
  // dest.h = size.y;
  // {
  //   if (::SDL_RenderCopyEx(sdl_renderer_,       //
  //                          color_attachment,    //
  //                          nullptr,             //
  //                          &dest,               //
  //                          180,                 //
  //                          NULL,                //
  //                          SDL_FLIP_HORIZONTAL  //
  //                          ) != 0) {
  //     return false;
  //   }
  // }

  ::SDL_RenderPresent(sdl_renderer_);
  return true;
}

bool Application::OnWindowSizeChanged(UPoint size) {
  return true;
}

}  // namespace ns
