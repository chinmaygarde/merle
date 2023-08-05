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

bool Application::OnRender() {
  if (!is_valid_) {
    std::cerr << "Application was invalid." << std::endl;
    return false;
  }

  if (!application_callback_) {
    return false;
  }

  const auto texture = application_callback_(*this);

  if (!texture) {
    return false;
  }

  const auto size = texture->GetSize();

  if (!onscreen_tex_.Resize(size)) {
    return false;
  }

  if (!texture->CopyRGBA(onscreen_tex_)) {
    return false;
  }

  SDLTextureNoCopyCaster sdl_texture(
      sdl_renderer_,                                      //
      onscreen_tex_.GetAllocation(),                      //
      size.x,                                             //
      size.y,                                             //
      static_cast<int>(onscreen_tex_.GetBytesPerPixel())  //
  );

  if (::SDL_RenderCopy(sdl_renderer_,  //
                       sdl_texture,    //
                       nullptr,        //
                       nullptr         //
                       ) != 0) {
    return false;
  }

  ::SDL_RenderPresent(sdl_renderer_);
  return true;
}

bool Application::OnWindowSizeChanged(UPoint size) {
  return true;
}

UPoint Application::GetWindowSize() const {
  return window_size_;
}

void Application::SetRasterizerCallback(ApplicationCallback callback) {
  application_callback_ = callback;
}

}  // namespace ns
