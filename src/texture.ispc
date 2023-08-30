
struct Color {
  uint8 red;
  uint8 green;
  uint8 blue;
  uint8 alpha;
};

enum Component {
  kRed,
  kGreen,
  kBlue,
  kAlpha,
};

struct Matrix {
  float e[4][4];
};

// This still end up being slower than direct memset on M1 MacBook Air.
export void Clear(uniform uint8 red[],
                  uniform uint8 green[],
                  uniform uint8 blue[],
                  uniform uint8 alpha[],
                  uniform const Color& color,
                  uniform uint64 size) {
  foreach (i = 0 ... size) {
    red[i] = color.red;
    green[i] = color.green;
    blue[i] = color.blue;
    alpha[i] = color.alpha;
  }
}

export void CopyToRGBA(uniform uint8 red[],
                       uniform uint8 green[],
                       uniform uint8 blue[],
                       uniform uint8 alpha[],
                       uniform Color rgba[],
                       uniform uint64 size) {
  foreach (i = 0 ... size) {
    Color c;
    c.red = red[i];
    c.green = green[i];
    c.blue = blue[i];
    c.alpha = alpha[i];
#pragma ignore warning(perf)  // scatter
    rgba[i] = c;
  }
}

export void FromRGBA(uniform Color rgba[],
                     uniform uint8 red[],
                     uniform uint8 green[],
                     uniform uint8 blue[],
                     uniform uint8 alpha[],
                     uniform uint64 size) {
  foreach (i = 0 ... size) {
#pragma ignore warning(perf)  // gather
    Color c = rgba[i];
    red[i] = c.red;
    green[i] = c.green;
    blue[i] = c.blue;
    alpha[i] = c.alpha;
  }
}

export void Grayscale(uniform uint8 reds[],
                      uniform uint8 greens[],
                      uniform uint8 blues[],
                      uniform uint64 size) {
  foreach (i = 0 ... size) {
    reds[i] = greens[i] = blues[i] =
        0.2126 * reds[i] + 0.7152 * greens[i] + 0.0722 * blues[i];
  }
}

export void Invert(uniform uint8 reds[],
                   uniform uint8 greens[],
                   uniform uint8 blues[],
                   uniform uint64 size) {
  foreach (i = 0 ... size) {
    reds[i] = 255 - reds[i];
    greens[i] = 255 - greens[i];
    blues[i] = 255 - blues[i];
  }
}

export void Exposure(uniform uint8 reds[],
                     uniform uint8 greens[],
                     uniform uint8 blues[],
                     uniform float exposure,
                     uniform uint64 size) {
  uniform float factor = pow(2, exposure);
  foreach (i = 0 ... size) {
    reds[i] = min(reds[i] * factor, 255.f);
    greens[i] = min(greens[i] * factor, 255.f);
    blues[i] = min(blues[i] * factor, 255.f);
  }
}

export void Brightness(uniform uint8 reds[],
                       uniform uint8 greens[],
                       uniform uint8 blues[],
                       uniform float exposure,
                       uniform uint64 size) {
  uniform uint8 factor = 255 * clamp(exposure, 0.0f, 1.0f);
  foreach (i = 0 ... size) {
    reds[i] = saturating_add(reds[i], factor);
    greens[i] = saturating_add(greens[i], factor);
    blues[i] = saturating_add(blues[i], factor);
  }
}

export void RGBALevels(uniform uint8 reds[],
                       uniform uint8 greens[],
                       uniform uint8 blues[],
                       uniform uint8 alphas[],
                       uniform float red_level,
                       uniform float green_level,
                       uniform float blue_level,
                       uniform float alpha_level,
                       uniform uint64 size) {
  red_level = max(red_level, 0.0f);
  green_level = max(green_level, 0.0f);
  blue_level = max(blue_level, 0.0f);
  alpha_level = max(alpha_level, 0.0f);
  foreach (i = 0 ... size) {
    reds[i] = min(reds[i] * red_level, 255.f);
    greens[i] = min(greens[i] * green_level, 255.f);
    blues[i] = min(blues[i] * blue_level, 255.f);
    alphas[i] = min(alphas[i] * alpha_level, 255.f);
  }
}

inline uint8 Select(uniform Component comp,
                    uint8 red,
                    uint8 green,
                    uint8 blue,
                    uint8 alpha) {
  switch (comp) {
    case kRed:
      return red;
    case kGreen:
      return green;
    case kBlue:
      return blue;
    case kAlpha:
      return alpha;
  }
  return 0;
}

export void Swizzle(uniform uint8 reds[],
                    uniform uint8 greens[],
                    uniform uint8 blues[],
                    uniform uint8 alphas[],
                    uniform Component red_swizzle,
                    uniform Component green_swizzle,
                    uniform Component blue_swizzle,
                    uniform Component alpha_swizzle,
                    uniform uint64 size) {
  foreach (i = 0 ... size) {
    uint8 red = reds[i];
    uint8 green = greens[i];
    uint8 blue = blues[i];
    uint8 alpha = alphas[i];

    reds[i] = Select(red_swizzle, red, green, blue, alpha);
    greens[i] = Select(green_swizzle, red, green, blue, alpha);
    blues[i] = Select(blue_swizzle, red, green, blue, alpha);
    alphas[i] = Select(alpha_swizzle, red, green, blue, alpha);
  }
}

export void ColorMatrix(uniform uint8 reds[],
                        uniform uint8 greens[],
                        uniform uint8 blues[],
                        uniform uint8 alphas[],
                        uniform int64 size,
                        uniform const Matrix& m) {
  foreach (i = 0 ... size) {
    float r = reds[i] / 255.0f;
    float g = greens[i] / 255.0f;
    float b = blues[i] / 255.0f;
    float a = alphas[i] / 255.0f;
    float r1 = r * m.e[0][0] + g * m.e[0][1] + b * m.e[0][2] + a * m.e[0][3];
    float g1 = r * m.e[1][0] + g * m.e[1][1] + b * m.e[1][2] + a * m.e[1][3];
    float b1 = r * m.e[2][0] + g * m.e[2][1] + b * m.e[2][2] + a * m.e[2][3];
    float a1 = r * m.e[3][0] + g * m.e[3][1] + b * m.e[3][2] + a * m.e[3][3];
    reds[i] = clamp(r1, 0.0f, 1.0f) * 255;
    greens[i] = clamp(g1, 0.0f, 1.0f) * 255;
    blues[i] = clamp(b1, 0.0f, 1.0f) * 255;
    alphas[i] = clamp(a1, 0.0f, 1.0f) * 255;
  }
}

export void Contrast(uniform uint8 reds[],
                     uniform uint8 greens[],
                     uniform uint8 blues[],
                     uniform int64 size,
                     uniform float contrast) {
  foreach (i = 0 ... size) {
    float r = reds[i] / 255.0f;
    float g = greens[i] / 255.0f;
    float b = blues[i] / 255.0f;
    reds[i] = clamp(((r - 0.5f) * contrast) + 0.5f, 0.0f, 1.0f) * 255;
    greens[i] = clamp(((g - 0.5f) * contrast) + 0.5f, 0.0f, 1.0f) * 255;
    blues[i] = clamp(((b - 0.5f) * contrast) + 0.5f, 0.0f, 1.0f) * 255;
  }
}

float Mix(float x, float y, float a) {
  return x * (1.0f - a) + y * a;
}

struct Vec3 {
  float x;
  float y;
  float z;
};

template <typename T1, typename T2>
inline float Dot3(const T1& v1, const T2& v2) {
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// https://en.wikipedia.org/wiki/Relative_luminance
static const uniform Vec3 kLuminanceWeights = {0.2126f, 0.7152f, 0.0722f};

export void Saturation(uniform uint8 reds[],
                       uniform uint8 greens[],
                       uniform uint8 blues[],
                       uniform int64 size,
                       uniform float saturation) {
  saturation = clamp(saturation + 1.0f, 0.0f, 2.0f);
  foreach (i = 0 ... size) {
    Vec3 color = {reds[i] / 255.0f, greens[i] / 255.0f, blues[i] / 255.0f};
    float luminance = Dot3(color, kLuminanceWeights);
    reds[i] = clamp(Mix(luminance, color.x, saturation), 0.0f, 1.0f) * 255;
    greens[i] = clamp(Mix(luminance, color.y, saturation), 0.0f, 1.0f) * 255;
    blues[i] = clamp(Mix(luminance, color.z, saturation), 0.0f, 1.0f) * 255;
  }
}

export void Vibrance(uniform uint8 reds[],
                     uniform uint8 greens[],
                     uniform uint8 blues[],
                     uniform int64 size,
                     uniform float vibrance) {
  vibrance = clamp(vibrance, -2.0f, 2.0f);
  foreach (i = 0 ... size) {
    float r = reds[i] / 255.0f;
    float g = greens[i] / 255.0f;
    float b = blues[i] / 255.0f;

    float average = (r + g + b) / 3.0f;
    float max = max(r, max(g, b));
    float amt = (max - average) * (-vibrance * 3.0f);

    reds[i] = Mix(r, max, amt) * 255;
    greens[i] = Mix(r, max, amt) * 255;
    blues[i] = Mix(r, max, amt) * 255;
  }
}

export void Hue(uniform uint8 reds[],
                uniform uint8 greens[],
                uniform uint8 blues[],
                uniform int64 size,
                uniform float hue_adjustment) {
  // See
  // http://stackoverflow.com/questions/9234724/how-to-change-hue-of-a-texture-with-glsl.
  uniform Vec3 kRGBToYPrime = {0.299, 0.587, 0.114};
  uniform Vec3 kRGBToI = {0.595716, -0.274453, -0.321263};
  uniform Vec3 kRGBToQ = {
      0.211456,
      -0.522591,
      0.31135,
  };
  uniform Vec3 kYIQToR = {1.0, 0.9563, 0.6210};
  uniform Vec3 kYIQToG = {1.0, -0.2721, -0.6474};
  uniform Vec3 kYIQToB = {1.0, -1.1070, 1.7046};

  foreach (i = 0 ... size) {
    Vec3 color = {reds[i] / 255.0f, greens[i] / 255.0f, blues[i] / 255.0f};

    // Convert to YIQ
    float YPrime = Dot3(color, kRGBToYPrime);
    float I = Dot3(color, kRGBToI);
    float Q = Dot3(color, kRGBToQ);

    // Calculate the hue and chroma
    float hue = atan2(Q, I);
    float chroma = sqrt(I * I + Q * Q);

    // Make the user's adjustments
    hue += hue_adjustment;

    // Convert back to YIQ
    Q = chroma * sin(hue);
    I = chroma * cos(hue);

    // Convert back to RGB
    Vec3 yIQ = {YPrime, I, Q};
    reds[i] = clamp(Dot3(yIQ, kYIQToR), 0.0f, 1.0f) * 255.0f;
    greens[i] = clamp(Dot3(yIQ, kYIQToG), 0.0f, 1.0f) * 255.0f;
    blues[i] = clamp(Dot3(yIQ, kYIQToB), 0.0f, 1.0f) * 255.0f;
  }
}

export void Opacity(uniform uint8 alphas[],
                    uniform int64 size,
                    uniform float opacity) {
  foreach (i = 0 ... size) {
    alphas[i] = ((alphas[i] / 255.0f) * opacity) * 255.0f;
  }
}

export uniform float AverageLuminance(uniform uint8 reds[],
                                      uniform uint8 greens[],
                                      uniform uint8 blues[],
                                      uniform int64 size) {
  double luma = 0.0;
  foreach (i = 0 ... size) {
    Vec3 c = {reds[i] / 255.0f, greens[i] / 255.0f, blues[i] / 255.0f};
    luma += Dot3(c, kLuminanceWeights);
  }
  return reduce_add(luma) / (double)size;
}

export void LuminanceThreshold(uniform uint8 reds[],
                               uniform uint8 greens[],
                               uniform uint8 blues[],
                               uniform int64 size,
                               uniform float luma_threshold) {
  foreach (i = 0 ... size) {
    Vec3 c = {reds[i] / 255.0f, greens[i] / 255.0f, blues[i] / 255.0f};
    float luma = Dot3(c, kLuminanceWeights);
    uint8 color = luma > luma_threshold ? 255 : 0;
    reds[i] = color;
    greens[i] = color;
    blues[i] = color;
  }
}

task void ConvolutionNxNTask(uniform uint8 src_r[],
                             uniform uint8 src_g[],
                             uniform uint8 src_b[],
                             uniform uint8 src_a[],
                             uniform uint8 dst_r[],
                             uniform uint8 dst_g[],
                             uniform uint8 dst_b[],
                             uniform uint8 dst_a[],
                             uniform int64 width,
                             uniform size_t y_begin,
                             uniform size_t y_end,
                             uniform float kernel[],
                             uniform int64 kernel_size) {
  uniform int64 kernel_width = sqrt((uniform float)kernel_size);
  uniform int64 radius = (kernel_width - 1u) / 2u;
  for (uniform size_t y = y_begin; y < y_end; y++) {
    foreach (x = radius...(width - radius)) {
      float sr = 0.0f;
      float sg = 0.0f;
      float sb = 0.0f;
      float sa = 0.0f;
      for (uniform int64 sy = -radius; sy < radius + 1; sy++) {
        for (uniform int64 sx = -radius; sx < radius + 1; sx++) {
          varying int64 offset = (width * (y + sy)) + x + sx;
          uniform float gauss =
              kernel[(sy + radius) * kernel_width + (sx + radius)];
          sr += src_r[offset] * gauss;
          sg += src_g[offset] * gauss;
          sb += src_b[offset] * gauss;
          sa += src_a[offset] * gauss;
        }
      }
      int64 offset = width * y + x;
      dst_r[offset] = sr;
      dst_g[offset] = sg;
      dst_b[offset] = sb;
      dst_a[offset] = sa;
    }
  }
}

export void ConvolutionNxN(uniform uint8 src_r[],
                           uniform uint8 src_g[],
                           uniform uint8 src_b[],
                           uniform uint8 src_a[],
                           uniform uint8 dst_r[],
                           uniform uint8 dst_g[],
                           uniform uint8 dst_b[],
                           uniform uint8 dst_a[],
                           uniform int64 width,
                           uniform int64 height,
                           uniform float kernel[],
                           uniform int64 kernel_size) {
  uniform int64 kernel_width = sqrt((uniform float)kernel_size);
  uniform int64 radius = (kernel_width - 1u) / 2u;
  uniform int64 y_window = height / num_cores();
  for (uniform size_t y = radius; y < height - radius; y += y_window) {
    launch ConvolutionNxNTask(src_r,                               //
                              src_g,                               //
                              src_b,                               //
                              src_a,                               //
                              dst_r,                               //
                              dst_g,                               //
                              dst_b,                               //
                              dst_a,                               //
                              width,                               //
                              y,                                   //
                              min(y + y_window, height - radius),  //
                              kernel,                              //
                              kernel_size                          //
    );
  }
}

export void Sobel(uniform uint8 src[],
                  uniform uint8 dst[],
                  uniform int64 width,
                  uniform int64 height) {
  // https://en.wikipedia.org/wiki/Sobel_operator
  static uniform uint8 xt[9] = {
      // clang-format off
    1, 0, -1,
    2, 0, -2,
    1, 0, -1
      // clang-format on
  };
  static uniform uint8 yt[9] = {
      // clang-format off
    1,  2,  1,
    0,  0,  0,
   -1, -2, -1
      // clang-format on
  };
#define SAMPLE(x, y) src[(y)*width + (x)]
#define SET(x, y) dst[(y)*width + (x)]
  for (uniform size_t y = 1; y < height - 1; y++) {
    foreach (x = 1...(width - 1)) {
      uint8 top_left = SAMPLE(x - 1, y - 1);
      uint8 top_mid = SAMPLE(x + 0, y - 1);
      uint8 top_right = SAMPLE(x + 1, y - 1);
      uint8 mid_left = SAMPLE(x - 1, y + 0);
      uint8 mid_mid = SAMPLE(x + 0, y + 0);
      uint8 mid_right = SAMPLE(x + 1, y + 0);
      uint8 low_left = SAMPLE(x - 1, y + 1);
      uint8 low_mid = SAMPLE(x + 0, y + 1);
      uint8 low_right = SAMPLE(x + 1, y + 1);

      // Calculate on X
      uint64 gx = 0;
      gx += top_left * xt[0];
      gx += top_mid * xt[1];
      gx += top_right * xt[2];
      gx += mid_left * xt[3];
      gx += mid_mid * xt[4];
      gx += mid_right * xt[5];
      gx += low_left * xt[6];
      gx += low_mid * xt[7];
      gx += low_right * xt[8];

#pragma ignore warning(perf)
      gx /= 8;

      // Calculate on Y
      uint64 gy = 0;
      gy += top_left * yt[0];
      gy += top_mid * yt[1];
      gy += top_right * yt[2];
      gy += mid_left * yt[3];
      gy += mid_mid * yt[4];
      gy += mid_right * yt[5];
      gy += low_left * yt[6];
      gy += low_mid * yt[7];
      gy += low_right * yt[8];

#pragma ignore warning(perf)
      gy /= 8;

#pragma ignore warning(perf)
      uint8 magnitude = sqrt((float)(gx * gx + gy * gy));

      SET(x, y) = magnitude;
    }
  }
#undef SET
#undef SAMPLE
}

export void Histogram(uniform uint8 r[],
                      uniform uint8 g[],
                      uniform uint8 b[],
                      uniform uint8 a[],
                      uniform size_t len,
                      uniform uint64 hr[],
                      uniform uint64 hg[],
                      uniform uint64 hb[],
                      uniform uint64 ha[]) {
  foreach (i = 0...len) {
    atomic_add_local(&hr[r[i]], 1u);
    atomic_add_local(&hg[g[i]], 1u);
    atomic_add_local(&hb[b[i]], 1u);
    atomic_add_local(&ha[a[i]], 1u);
  }
}

inline uint8 Mix(uint8 x, uint8 y, uniform float t) {
  return x * (1.0f - t) + y * t;
}

export void FadeTransition(uniform uint8 dst_r[],
                           uniform uint8 dst_g[],
                           uniform uint8 dst_b[],
                           uniform uint8 dst_a[],
                           uniform uint8 from_r[],
                           uniform uint8 from_g[],
                           uniform uint8 from_b[],
                           uniform uint8 from_a[],
                           uniform uint8 to_r[],
                           uniform uint8 to_g[],
                           uniform uint8 to_b[],
                           uniform uint8 to_a[],
                           uniform size_t len,
                           uniform float t) {
  foreach (i = 0 ... len) {
    dst_r[i] = Mix(from_r[i], to_r[i], t);
    dst_g[i] = Mix(from_g[i], to_g[i], t);
    dst_b[i] = Mix(from_b[i], to_b[i], t);
    dst_a[i] = Mix(from_a[i], to_a[i], t);
  }
}

export void SwipeTransitionHorizontal(uniform uint8 dst_r[],
                                      uniform uint8 dst_g[],
                                      uniform uint8 dst_b[],
                                      uniform uint8 dst_a[],
                                      uniform uint8 from_r[],
                                      uniform uint8 from_g[],
                                      uniform uint8 from_b[],
                                      uniform uint8 from_a[],
                                      uniform uint8 to_r[],
                                      uniform uint8 to_g[],
                                      uniform uint8 to_b[],
                                      uniform uint8 to_a[],
                                      uniform size_t width,
                                      uniform size_t height,
                                      uniform float t) {
  uniform size_t x_break = width * t;
  for (uniform size_t y = 0; y < height; y++) {
    foreach (x = 0...x_break) {
      size_t offset = width * y + x;
      dst_r[offset] = from_r[offset];
      dst_g[offset] = from_g[offset];
      dst_b[offset] = from_b[offset];
      dst_a[offset] = from_a[offset];
    }
  }
  for (uniform size_t y = 0; y < height; y++) {
    foreach (x = x_break... width) {
      size_t offset = width * y + x;
      dst_r[offset] = to_r[offset];
      dst_g[offset] = to_g[offset];
      dst_b[offset] = to_b[offset];
      dst_a[offset] = to_a[offset];
    }
  }
}

export void SwipeTransitionVertical(uniform uint8 dst_r[],
                                    uniform uint8 dst_g[],
                                    uniform uint8 dst_b[],
                                    uniform uint8 dst_a[],
                                    uniform uint8 from_r[],
                                    uniform uint8 from_g[],
                                    uniform uint8 from_b[],
                                    uniform uint8 from_a[],
                                    uniform uint8 to_r[],
                                    uniform uint8 to_g[],
                                    uniform uint8 to_b[],
                                    uniform uint8 to_a[],
                                    uniform size_t width,
                                    uniform size_t height,
                                    uniform float t) {
  uniform size_t y_break = height * t;
  for (uniform size_t y = 0; y < y_break; y++) {
    foreach (x = 0...width) {
      size_t offset = width * y + x;
      dst_r[offset] = from_r[offset];
      dst_g[offset] = from_g[offset];
      dst_b[offset] = from_b[offset];
      dst_a[offset] = from_a[offset];
    }
  }
  for (uniform size_t y = y_break; y < height; y++) {
    foreach (x = 0...width) {
      size_t offset = width * y + x;
      dst_r[offset] = to_r[offset];
      dst_g[offset] = to_g[offset];
      dst_b[offset] = to_b[offset];
      dst_a[offset] = to_a[offset];
    }
  }
}

export void AverageColor(uniform uint8 r[],
                         uniform uint8 g[],
                         uniform uint8 b[],
                         uniform uint8 a[],
                         uniform uint64 size,
                         uniform Color& out_color) {
  uint64 red_sum = 0;
  uint64 green_sum = 0;
  uint64 blue_sum = 0;
  uint64 alpha_sum = 0;
  foreach (i = 0 ... size) {
    red_sum += r[i];
    green_sum += g[i];
    blue_sum += b[i];
    alpha_sum += a[i];
  }
  out_color.red = reduce_add(red_sum) / size;
  out_color.green = reduce_add(green_sum) / size;
  out_color.blue = reduce_add(blue_sum) / size;
  out_color.alpha = reduce_add(alpha_sum) / size;
}

export uniform bool AllEqual(uniform uint8 c[],
                             uniform uint64 size,
                             uniform uint8 val) {
  bool eq = false;
  uniform int rem = size % programCount;
  for (uniform size_t i = 0; i < size - rem; i += programCount) {
    eq |= c[i + programIndex] == val;
  }
  if (!all(eq)) {
    return false;
  }
  for (uniform size_t i = size - rem; i < size; i++) {
    if (c[i] != val) {
      return false;
    }
  }
  return true;
}
