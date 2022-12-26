#include <arm_neon.h>
#include <gtest/gtest.h>
#include "application.h"
#include "fixtures_location.h"
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
        texture->Clear(kColorBlue);
        return texture;
      });
  ASSERT_TRUE(Run(application));
}

void DebugBreak() {}

TEST_F(NeonSandboxTest, TestGrayscale) {
  DebugBreak();
  std::vector<uint32_t> items{
      0,  1,  2,  3,   //
      4,  5,  6,  7,   //
      8,  9,  10, 11,  //
      12, 13, 14, 15,  //
  };

  auto demo = vld4q_u32(items.data());

  // Colors are BGRA
  std::vector<Color> colors{
      kColorBlue, kColorBlue, kColorBlue, kColorBlue,  //
      kColorBlue, kColorBlue, kColorBlue, kColorBlue,  //
      kColorBlue, kColorBlue, kColorBlue, kColorBlue,  //
      kColorBlue, kColorBlue, kColorBlue, kColorBlue,  //
  };

  uint8x8x4_t reg = vld4_u8((uint8_t*)colors.data());

  uint8x8_t scale = vdup_n_u8(85);

  uint16x8_t temp = vmull_u8(reg.val[0], scale);
  temp = vmlal_u8(temp, reg.val[1], scale);
  temp = vmlal_u8(temp, reg.val[2], scale);

  uint8x8_t gray = vshrn_n_u16(temp, 8u);

  // vst1_u8()
  uint8x8_t color = reg.val[0];
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 8; x++) {
      std::cout << (int)color[y * 8 + x] << ",";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

}  // namespace ns
