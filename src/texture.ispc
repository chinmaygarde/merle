
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

export void ToRGBA(uniform uint8 red[],
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

export void ToGrayscale(uniform uint8 reds[],
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

export void Saturation(uniform uint8 reds[],
                       uniform uint8 greens[],
                       uniform uint8 blues[],
                       uniform int64 size,
                       uniform float saturation) {
  uniform Vec3 luminance_weights = {0.2125f, 0.7154f, 0.0721f};
  saturation = clamp(saturation + 1.0f, 0.0f, 2.0f);
  foreach (i = 0 ... size) {
    Vec3 color = {reds[i] / 255.0f, greens[i] / 255.0f, blues[i] / 255.0f};
    float luminance = Dot3(color, luminance_weights);
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
