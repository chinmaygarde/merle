#include "benchmark/benchmark.h"
#include "geom.h"
#include "texture.h"

namespace merle {

static constexpr UPoint kBenchmarkCanvasSize = {1 << 14, 1 << 14};

static void DoNothing(benchmark::State& state) {
  while (state.KeepRunning()) {
    //
  }
}
BENCHMARK(DoNothing)->Unit(benchmark::TimeUnit::kMillisecond);

static void Clear(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Clear(kColorBlue);
  }
}
BENCHMARK(Clear)->Unit(benchmark::TimeUnit::kMillisecond);

static void ToRGBA(benchmark::State& state) {
  Texture texture;
  Texture rgba;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(rgba.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorBlue);
  while (state.KeepRunning()) {
    texture.CopyToRGBA(rgba);
  }
}
BENCHMARK(ToRGBA)->Unit(benchmark::TimeUnit::kMillisecond);

static void Grayscale(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Grayscale();
  }
}
BENCHMARK(Grayscale)->Unit(benchmark::TimeUnit::kMillisecond);

static void Invert(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Invert();
  }
}
BENCHMARK(Invert)->Unit(benchmark::TimeUnit::kMillisecond);

static void Exposure(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Exposure(2.0f);
  }
}
BENCHMARK(Exposure)->Unit(benchmark::TimeUnit::kMillisecond);

static void Brightness(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Brightness(0.5f);
  }
}
BENCHMARK(Brightness)->Unit(benchmark::TimeUnit::kMillisecond);

static void RGBALevels(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.RGBALevels(0.5f, 1.0, 0.0, 1.0f);
  }
}
BENCHMARK(RGBALevels)->Unit(benchmark::TimeUnit::kMillisecond);

static void Swizzle(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
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
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Sepia();
  }
}
BENCHMARK(Sepia)->Unit(benchmark::TimeUnit::kMillisecond);

static void Contrast(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Contrast(3.0f);
  }
}
BENCHMARK(Contrast)->Unit(benchmark::TimeUnit::kMillisecond);

static void Saturation(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Saturation(.05f);
  }
}
BENCHMARK(Saturation)->Unit(benchmark::TimeUnit::kMillisecond);

static void Vibrance(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Vibrance(1.5f);
  }
}
BENCHMARK(Vibrance)->Unit(benchmark::TimeUnit::kMillisecond);

static void Hue(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Hue(Degrees{90});
  }
}
BENCHMARK(Hue)->Unit(benchmark::TimeUnit::kMillisecond);

static void Opacity(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Opacity(0.5f);
  }
}
BENCHMARK(Opacity)->Unit(benchmark::TimeUnit::kMillisecond);

static void AverageLuminance(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    texture.AverageLuminance();
  }
}
BENCHMARK(AverageLuminance)->Unit(benchmark::TimeUnit::kMillisecond);

static void LuminanceThreshold(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    texture.LuminanceThreshold(0.5f);
  }
}
BENCHMARK(LuminanceThreshold)->Unit(benchmark::TimeUnit::kMillisecond);

static void BoxBlur(benchmark::State& state) {
  Texture texture;
  Texture blur;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(blur.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  blur.Clear(kColorBlack);
  while (state.KeepRunning()) {
    blur.BoxBlur(texture, 2);
  }
}
BENCHMARK(BoxBlur)->Unit(benchmark::TimeUnit::kMillisecond);

static void GaussianBlur(benchmark::State& state) {
  Texture texture;
  Texture blur;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(blur.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  blur.Clear(kColorBlack);
  while (state.KeepRunning()) {
    blur.GaussianBlur(texture, 2, 4.0f);
  }
}
BENCHMARK(GaussianBlur)->Unit(benchmark::TimeUnit::kMillisecond);

static void Sobel(benchmark::State& state) {
  Texture texture;
  Texture sobel;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(sobel.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  sobel.Clear(kColorBlack);
  while (state.KeepRunning()) {
    sobel.Sobel(texture, Component::kRed, Component::kRed);
  }
}
BENCHMARK(Sobel)->Unit(benchmark::TimeUnit::kMillisecond);

static void DuplicateChannel(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    texture.DuplicateChannel(Component::kRed, Component::kBlue);
  }
}
BENCHMARK(DuplicateChannel)->Unit(benchmark::TimeUnit::kMillisecond);

static void FadeTransition(benchmark::State& state) {
  Texture a, b, c;
  MERLE_ASSERT(a.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(b.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(c.Resize(kBenchmarkCanvasSize));
  a.Clear(kColorFuchsia);
  b.Clear(kColorBlue);
  c.Clear(kColorRed);
  while (state.KeepRunning()) {
    c.FadeTransition(a, b, 0.75);
  }
}
BENCHMARK(FadeTransition)->Unit(benchmark::TimeUnit::kMillisecond);

static void SwipeTransitionHorizontal(benchmark::State& state) {
  Texture a, b, c;
  MERLE_ASSERT(a.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(b.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(c.Resize(kBenchmarkCanvasSize));
  a.Clear(kColorFuchsia);
  b.Clear(kColorBlue);
  c.Clear(kColorRed);
  while (state.KeepRunning()) {
    c.SwipeTransition(a, b, 0.75, Texture::Direction::kHorizontal);
  }
}
BENCHMARK(SwipeTransitionHorizontal)->Unit(benchmark::TimeUnit::kMillisecond);

static void SwipeTransitionVertical(benchmark::State& state) {
  Texture a, b, c;
  MERLE_ASSERT(a.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(b.Resize(kBenchmarkCanvasSize));
  MERLE_ASSERT(c.Resize(kBenchmarkCanvasSize));
  a.Clear(kColorFuchsia);
  b.Clear(kColorBlue);
  c.Clear(kColorRed);
  while (state.KeepRunning()) {
    c.SwipeTransition(a, b, 0.75, Texture::Direction::kVertical);
  }
}
BENCHMARK(SwipeTransitionVertical)->Unit(benchmark::TimeUnit::kMillisecond);

static void AverageColor(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    texture.AverageColor();
  }
}
BENCHMARK(AverageColor)->Unit(benchmark::TimeUnit::kMillisecond);

static void IsOpaque(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite);
  while (state.KeepRunning()) {
    MERLE_ASSERT(texture.IsOpaque());
  }
}
BENCHMARK(IsOpaque)->Unit(benchmark::TimeUnit::kMillisecond);

static void PremultiplyAlpha(benchmark::State& state) {
  Texture texture;
  MERLE_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorWhite.WithAlpha(128));
  while (state.KeepRunning()) {
    texture.PremultiplyAlpha();
  }
}
BENCHMARK(PremultiplyAlpha)->Unit(benchmark::TimeUnit::kMillisecond);

}  // namespace merle

BENCHMARK_MAIN();
