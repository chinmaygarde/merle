#include "test_runner.h"

#include <SDL3/SDL.h>

#include <sstream>

#include "application.h"
#include "fixtures_location.h"

namespace ns {

TestRunner::TestRunner() {
  NS_ASSERT(::SDL_Init(SDL_INIT_VIDEO) == 0);
}

static bool gSkipRemainingTests = false;

bool TestRunner::Run(Application& application) const {
  bool is_running = true;
  bool success = true;
  while (is_running) {
    success = is_running = application.Render();
    ::SDL_Event event;
    if (::SDL_PollEvent(&event) == 1) {
      switch (event.type) {
        case SDL_KEYUP:
          switch (event.key.keysym.sym) {
            case SDL_KeyCode::SDLK_q:
            case SDL_KeyCode::SDLK_ESCAPE:
              if ((event.key.keysym.mod & KMOD_LSHIFT) ||
                  (event.key.keysym.mod & KMOD_RSHIFT) ||
                  (event.key.keysym.mod & KMOD_LCTRL) ||
                  (event.key.keysym.mod & KMOD_RCTRL)) {
                gSkipRemainingTests = true;
              }
              is_running = false;
              break;
            default:
              break;
          }
          break;
        case SDL_QUIT:
          is_running = false;
          break;
        case SDL_WINDOWEVENT:
          switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
              UPoint size;
              if (event.window.data1 > 0) {
                size.x = event.window.data1;
              }
              if (event.window.data2 > 0) {
                size.y = event.window.data2;
              }
              if (!application.OnWindowSizeChanged(size)) {
                std::cout << "Window resizing failed." << std::endl;
                is_running = false;
              }
              break;
          }
          break;
      }
    }
  }
  return success;
}

TestRunner::~TestRunner() {
  ::SDL_Quit();
}

void TestRunner::SetUp() {
  if (gSkipRemainingTests) {
    GTEST_SKIP();
  }
}

void TestRunner::TearDown() {}

}  // namespace ns
