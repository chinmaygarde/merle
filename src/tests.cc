#include <gtest/gtest.h>
#include "application.h"
#include "fixtures_location.h"
#include "simd/register.h"
#include "test_runner.h"
#include "texture.h"

namespace ns {

using NeonSandboxTest = TestRunner;

TEST_F(NeonSandboxTest, Setup) {
  Application application;
  ASSERT_FALSE(Run(application));
}

TEST_F(NeonSandboxTest, CanCreateTexture) {
  Texture texture;
  ASSERT_TRUE(texture.Resize({1, 1}));
  texture[{0, 0}] = kColorRoyalBlue;
  ASSERT_EQ(texture[UPoint(0, 0)], kColorRoyalBlue);
}

TEST_F(NeonSandboxTest, CanDisplayTexture) {
  Application application;
  auto texture = std::make_shared<Texture>();
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorAqua);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(NeonSandboxTest, CanConvertToGrayScale) {
  Application application;
  auto texture = std::make_shared<Texture>();
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        for (size_t y = 0; y < size.y; y++) {
          for (size_t x = 0; x < size.x; x++) {
            (*texture)[UPoint(x, y)] = Color::FromComponentsF(
                static_cast<ScalarF>(x) / size.x,
                static_cast<ScalarF>(x + y) / size.x + size.y,
                static_cast<ScalarF>(y) / size.y, 1.0f);
          }
        }
        texture->ConvertToGrayScale();
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(NeonSandboxTest, CanCreateRegisters) {
  RegisterT<uint32_t, 4> reg(8u);
}

}  // namespace ns
