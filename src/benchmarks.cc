#include "benchmark/benchmark.h"
#include "texture.h"

namespace ns {

static constexpr UPoint kBenchmarkCanvasSize = {1 << 14, 1 << 14};

static void BM_DoNothing(benchmark::State& state) {
  while (state.KeepRunning()) {
    //
  }
}
BENCHMARK(BM_DoNothing)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_Clear(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Clear(kColorBlue);
  }
}
BENCHMARK(BM_Clear)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_ToRGBA(benchmark::State& state) {
  Texture texture;
  Texture rgba;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  NS_ASSERT(rgba.Resize(kBenchmarkCanvasSize));
  texture.Clear(kColorBlue);
  while (state.KeepRunning()) {
    texture.CopyRGBA(rgba);
  }
}
BENCHMARK(BM_ToRGBA)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_Grayscale(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.ToGrayscale();
  }
}
BENCHMARK(BM_Grayscale)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_Invert(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Invert();
  }
}
BENCHMARK(BM_Invert)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_Exposure(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize(kBenchmarkCanvasSize));
  while (state.KeepRunning()) {
    texture.Exposure(2.0f);
  }
}
BENCHMARK(BM_Exposure)->Unit(benchmark::TimeUnit::kMillisecond);

}  // namespace ns

BENCHMARK_MAIN();
