#include "application.h"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>
#include "SDL_render.h"
#include "SDL_video.h"

namespace ns {

Application::Application(UPoint size) {
  window_size_ = size;

  // Setup SDL window and renderer.
  sdl_window_ = ::SDL_CreateWindow(
      "Neon Sandbox",                                               //
      window_size_.x,                                               //
      window_size_.y,                                               //
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN  //
  );

  if (!sdl_window_) {
    std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
    return;
  }

  sdl_renderer_ = ::SDL_CreateRenderer(
      sdl_window_,                                          //
      nullptr,                                              // driver name
      SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED  //
  );

  SDL_SetWindowSize(sdl_window_, 1280, 900);
  SDL_SetWindowPosition(sdl_window_, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(sdl_window_);

  if (!sdl_renderer_) {
    std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
    return;
  }

  // Setup ImGUI context.
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL3_InitForSDLRenderer(sdl_window_, sdl_renderer_);
  ImGui_ImplSDLRenderer3_Init(sdl_renderer_);

  is_valid_ = true;
}

Application::~Application() {
  if (is_valid_) {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
  }
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

  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  const auto texture = application_callback_(*this);

  ImGui::Render();

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

  ::SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 255);
  ::SDL_RenderClear(sdl_renderer_);
  if (::SDL_RenderTexture(sdl_renderer_,  //
                          sdl_texture,    //
                          nullptr,        //
                          nullptr         //
                          ) != 0) {
    return false;
  }
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
  ::SDL_RenderPresent(sdl_renderer_);
  return true;
}

bool Application::OnWindowSizeChanged(UPoint size) {
  window_size_ = size;
  return true;
}

UPoint Application::GetWindowSize() const {
  return window_size_;
}

void Application::SetRasterizerCallback(ApplicationCallback callback) {
  application_callback_ = callback;
}

}  // namespace ns
