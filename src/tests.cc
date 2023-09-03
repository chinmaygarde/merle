#include <gtest/gtest.h>

#include <imgui.h>
#include <memory>
#include "application.h"
#include "fixtures_location.h"
#include "geom.h"
#include "test_runner.h"
#include "texture.h"

namespace merle {

using MerleTest = TestRunner;

TEST_F(MerleTest, Setup) {
  Application application;
  ASSERT_FALSE(Run(application));
}

TEST_F(MerleTest, Clear) {
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

TEST_F(MerleTest, Replace) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        static int xy[2] = {25, 25};
        const auto image_size = image->GetSize();
        ImGui::SliderInt2("Offset", xy, -image_size.x - 25, size.x + 25);
        // const auto offset = Point{static_cast<int32_t>(std::max(0, xy[0])),
        //                           static_cast<int32_t>(std::max(0, xy[1]))};
        texture->Replace(*image, Point{xy[0], xy[1]});
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Grayscale) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        texture->Grayscale();
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Invert) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "kalimba.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        texture->Invert();
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Exposure) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "kalimba.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        static float exposure = 0.0f;
        ImGui::SliderFloat("Exposure", &exposure, -2.0f, 2.0f);
        texture->Exposure(exposure);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Brightness) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "kalimba.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        static float brightness = 0.0f;
        ImGui::SliderFloat("Brightness", &brightness, 0.0f, 3.0f);
        texture->Brightness(brightness);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, RGBALevels) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        static float vals[] = {1.0f, 1.0f, 1.0f, 1.0f};
        ImGui::SliderFloat4("RGBA Levels", vals, 0.0f, 1.0f);
        texture->RGBALevels(vals[0], vals[1], vals[2], vals[3]);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Swizzle) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback([&](const Application& app)
                                        -> std::shared_ptr<Texture> {
    const auto size = app.GetWindowSize();
    if (!texture->Resize(size)) {
      return nullptr;
    }
    texture->Clear(Color{0, 0, 255, 255});
    texture->Replace(*image, {25, 25});
    static const char* kComponentNames[] = {"Red", "Green", "Blue", "Alpha"};
    static int red = 0;
    ImGui::Combo("Red", &red, kComponentNames, IM_ARRAYSIZE(kComponentNames));
    static int green = 0;
    ImGui::Combo("Green", &green, kComponentNames,
                 IM_ARRAYSIZE(kComponentNames));
    static int blue = 2;
    ImGui::Combo("Blue", &blue, kComponentNames, IM_ARRAYSIZE(kComponentNames));
    static int alpha = 3;
    ImGui::Combo("Alpha", &alpha, kComponentNames,
                 IM_ARRAYSIZE(kComponentNames));
    texture->Swizzle(static_cast<Component>(red), static_cast<Component>(green),
                     static_cast<Component>(blue),
                     static_cast<Component>(alpha));
    return texture;
  });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Sepia) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        texture->Sepia();
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Contrast) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(Color{0, 0, 255, 255});
        texture->Replace(*image, {25, 25});
        static float contrast = 1.0f;
        ImGui::SliderFloat("Contrast", &contrast, 0.0f, 4.0f);
        texture->Contrast(contrast);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Saturation) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});
        static float saturation = 0.0f;
        ImGui::SliderFloat("Saturation", &saturation, -1.0f, 1.0f);
        texture->Saturation(saturation);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Vibrance) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});
        static float vibrance = 0.0f;
        ImGui::SliderFloat("Vibrance", &vibrance, -2.0f, 2.0f);
        texture->Saturation(vibrance);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Hue) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});
        static float hue = 0.0f;
        ImGui::SliderFloat("Hue Adjustment (Degrees)", &hue, 0.0f, 360.0f);
        texture->Hue(Degrees{hue});
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Opacity) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});
        static float opacity = 0.5f;
        ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f);
        texture->Opacity(opacity);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, AverageLuminance) {
  Texture texture;
  ASSERT_TRUE(texture.Resize({1 << 14, 1 << 14}));
  texture.Clear(kColorWhite);
  ASSERT_EQ(texture.AverageLuminance(), 1.0f);
}

TEST_F(MerleTest, LuminanceThreshold) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});
        static float luma = texture->AverageLuminance();
        ImGui::SliderFloat("Luminance Threshold", &luma, 0.0f, 1.0f);
        texture->LuminanceThreshold(luma);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, BoxBlur) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto blur_texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size) || !blur_texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});

        static int radius = 1;
        ImGui::SliderInt("Radius", &radius, 0, 4);
        blur_texture->BoxBlur(*texture, radius);

        return blur_texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, GaussianBlur) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto blur_texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "civic_center.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size) || !blur_texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {25, 25});

        static int radius = 1;
        ImGui::SliderInt("Blur Radius", &radius, 0, 15);
        static float sigma = 1.5f;
        ImGui::SliderFloat("Gaussian Blur Sigma", &sigma, 0.1f, 20.0f);
        blur_texture->GaussianBlur(*texture, radius, sigma);

        return blur_texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, Sobel) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto blur_texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size) || !blur_texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(kColorBlack);
        texture->Replace(*image, {0, 0});
        texture->Grayscale();

        blur_texture->Clear(kColorRed);
        blur_texture->Sobel(*texture, Component::kRed, Component::kRed);
        blur_texture->DuplicateChannel(Component::kRed, Component::kGreen);
        blur_texture->DuplicateChannel(Component::kRed, Component::kBlue);

        return blur_texture;
      });
  ASSERT_TRUE(Run(application));
}

static std::shared_ptr<Texture> CreateSizedImage(const char* path,
                                                 UPoint size,
                                                 Color bg_color) {
  auto src = Texture::CreateFromFile(path);
  if (!src.has_value()) {
    return nullptr;
  }
  auto tex = std::make_shared<Texture>();
  if (!tex->Resize(size)) {
    return nullptr;
  }
  tex->Clear(bg_color);
  tex->Replace(src.value(), {});
  return tex;
}

TEST_F(MerleTest, FadeTransition) {
  Application application;
  auto boston = CreateSizedImage(NS_ASSETS_LOCATION "boston.jpg", {800, 600},
                                 kColorBlack);
  auto kalimba = CreateSizedImage(NS_ASSETS_LOCATION "kalimba.jpg", {800, 600},
                                  kColorBlack);
  auto dst = std::make_shared<Texture>();
  ASSERT_TRUE(dst->Resize({800, 600}));
  ASSERT_TRUE(boston && kalimba);
  auto texture = std::make_shared<Texture>();
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        static float transition = 0.25f;
        ImGui::SliderFloat("Transition", &transition, 0.0f, 1.0f);
        dst->FadeTransition(*boston, *kalimba, transition);
        texture->Clear(kColorBlack);
        texture->Replace(*dst, {10, 10});

        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, SwipeTransition) {
  Application application;
  auto boston = CreateSizedImage(NS_ASSETS_LOCATION "boston.jpg", {800, 600},
                                 kColorBlack);
  auto kalimba = CreateSizedImage(NS_ASSETS_LOCATION "kalimba.jpg", {800, 600},
                                  kColorBlack);
  auto dst = std::make_shared<Texture>();
  ASSERT_TRUE(dst->Resize({800, 600}));
  ASSERT_TRUE(boston && kalimba);
  auto texture = std::make_shared<Texture>();
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        static float transition = 0.25f;
        ImGui::SliderFloat("Transition", &transition, 0.0f, 1.0f);
        static const char* kDirectionNames[] = {"Horizontal", "Vertical"};
        static int dir = 0;
        ImGui::Combo("Direction", &dir, kDirectionNames,
                     IM_ARRAYSIZE(kDirectionNames));
        dst->SwipeTransition(*boston, *kalimba, transition,
                             static_cast<Texture::Direction>(dir));
        texture->Clear(kColorBlack);
        texture->Replace(*dst, {10, 10});

        return texture;
      });
  ASSERT_TRUE(Run(application));
}

TEST_F(MerleTest, AverageColor) {
  Texture texture;
  ASSERT_TRUE(texture.Resize({500, 500}));
  texture.Clear(kColorFuchsia);
  ASSERT_EQ(texture.AverageColor().red, kColorFuchsia.red);
  ASSERT_EQ(texture.AverageColor().green, kColorFuchsia.green);
  ASSERT_EQ(texture.AverageColor().blue, kColorFuchsia.blue);
  ASSERT_EQ(texture.AverageColor().alpha, kColorFuchsia.alpha);
}

TEST_F(MerleTest, AverageColorApply) {
  Application application;
  auto texture = std::make_shared<Texture>();
  auto image = Texture::CreateFromFile(NS_ASSETS_LOCATION "boston.jpg");
  ASSERT_TRUE(image.has_value());
  auto average = image->AverageColor();
  application.SetRasterizerCallback(
      [&](const Application& app) -> std::shared_ptr<Texture> {
        const auto size = app.GetWindowSize();
        if (!texture->Resize(size)) {
          return nullptr;
        }
        texture->Clear(average);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

}  // namespace merle
