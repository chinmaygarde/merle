#include "benchmark/benchmark.h"
#include "texture.h"

namespace ns {

static void BM_DoNothing(benchmark::State& state) {
  while (state.KeepRunning()) {
    //
  }
}
BENCHMARK(BM_DoNothing)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_Clear(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize({8192, 8192}));
  while (state.KeepRunning()) {
    texture.Clear(kColorBlue);
  }
}
BENCHMARK(BM_Clear)->Unit(benchmark::TimeUnit::kMillisecond);

static void BM_Grayscale(benchmark::State& state) {
  Texture texture;
  NS_ASSERT(texture.Resize({8192, 8192}));
  while (state.KeepRunning()) {
    texture.ToGrayscale();
  }
}
BENCHMARK(BM_Grayscale)->Unit(benchmark::TimeUnit::kMillisecond);

}  // namespace ns

BENCHMARK_MAIN();
