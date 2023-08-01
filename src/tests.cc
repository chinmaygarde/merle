#include <gtest/gtest.h>
#include "application.h"
#include "fixtures_location.h"
#include "geom.h"
#include "test_runner.h"
#include "texture.h"

namespace ns {

using NeonSandboxTest = TestRunner;

TEST_F(NeonSandboxTest, Setup) {
  Application application;
  ASSERT_FALSE(Run(application));
}

TEST_F(NeonSandboxTest, Scratch) {
  Texture texture;
  texture.Resize({1, 1});
  texture[{0, 0}] = kColorRed;
  texture.ToGrayscale();
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
        texture->Clear(kColorYellow);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(NeonSandboxTest, CanDisplayGrayscaleTexture) {
  Application application;
  auto texture = std::make_shared<Texture>();
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlue);
        texture->ToGrayscale();
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

}  // namespace ns
