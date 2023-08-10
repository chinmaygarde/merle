#include "CMakeFiles/neon_sandbox_ispc.dir/texture_ispc.h"
#include "benchmark/benchmark.h"
#include "geom.h"
#include "texture.h"

namespace ns {

static constexpr UPoint kBenchmarkCanvasSize = {1 << 14, 1 << 14};

static void DoNothing(benchmark::State& state) {
  while (state.KeepRunning()) {
    //
  }
}
BENCHMARK(DoNothing)->Unit(benchmark::TimeUnit::kMillisecond);

static void Clear(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Clear(kColorBlue);
  }
}
BENCHMARK(Clear)->Unit(benchmark::TimeUnit::kMillisecond);

static void ToRGBA(benchmark::State& state) {
  Texture texture;
  Texture rgba;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  NS_ASSERT(rgba.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorBlue);
  while (state.KeepRunning()) {
    texture.CopyToRGBA(rgba);
  }
}
BENCHMARK(ToRGBA)->Unit(benchmark::TimeUnit::kMillisecond);

static void Grayscale(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Grayscale();
  }
}
BENCHMARK(Grayscale)->Unit(benchmark::TimeUnit::kMillisecond);

static void Invert(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Invert();
  }
}
BENCHMARK(Invert)->Unit(benchmark::TimeUnit::kMillisecond);

static void Exposure(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Exposure(2.0f);
  }
}
BENCHMARK(Exposure)->Unit(benchmark::TimeUnit::kMillisecond);

static void Brightness(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Brightness(0.5f);
  }
}
BENCHMARK(Brightness)->Unit(benchmark::TimeUnit::kMillisecond);

static void RGBALevels(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.RGBALevels(0.5f, 1.0, 0.0, 1.0f);
  }
}
BENCHMARK(RGBALevels)->Unit(benchmark::TimeUnit::kMillisecond);

static void Swizzle(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Swizzle(Component::kAlpha,  //
                    Component::kRed,    //
                    Component::kGreen,  //
                    Component::kAlpha   //
    );
  }
}
BENCHMARK(Swizzle)->Unit(benchmark::TimeUnit::kMillisecond);

static void Sepia(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Sepia();
  }
}
BENCHMARK(Sepia)->Unit(benchmark::TimeUnit::kMillisecond);

static void Contrast(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Contrast(3.0f);
  }
}
BENCHMARK(Contrast)->Unit(benchmark::TimeUnit::kMillisecond);

static void Saturation(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Saturation(.05f);
  }
}
BENCHMARK(Saturation)->Unit(benchmark::TimeUnit::kMillisecond);

static void Vibrance(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Vibrance(1.5f);
  }
}
BENCHMARK(Vibrance)->Unit(benchmark::TimeUnit::kMillisecond);

static void Hue(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Hue(Degrees{90});
  }
}
BENCHMARK(Hue)->Unit(benchmark::TimeUnit::kMillisecond);

static void Opacity(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Opacity(0.5f);
  }
}
BENCHMARK(Opacity)->Unit(benchmark::TimeUnit::kMillisecond);

static void AverageLuminance(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    texture.AverageLuminance();
  }
}
BENCHMARK(AverageLuminance)->Unit(benchmark::TimeUnit::kMillisecond);

static void LuminanceThreshold(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    texture.LuminanceThreshold(0.5f);
  }
}
BENCHMARK(LuminanceThreshold)->Unit(benchmark::TimeUnit::kMillisecond);

static void BoxBlur(benchmark::State& state) {
  Texture texture;
  Texture blur;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  NS_ASSERT(blur.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  texture.Clear(kColorBlack);
  while (state.KeepRunning()) {
    blur.BoxBlur(texture, 2);
  }
}
BENCHMARK(BoxBlur)->Unit(benchmark::TimeUnit::kMillisecond);

static void GaussianBlur(benchmark::State& state) {
  Texture texture;
  Texture blur;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  NS_ASSERT(blur.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  texture.Clear(kColorBlack);
  while (state.KeepRunning()) {
    blur.GaussianBlur(texture, 2, 4.0f);
  }
}
BENCHMARK(GaussianBlur)->Unit(benchmark::TimeUnit::kMillisecond);

}  // namespace ns

BENCHMARK_MAIN();
