
struct Color {
  uint8 red;
  uint8 green;
  uint8 blue;
  uint8 alpha;
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
  uniform uint8 factor = 255 * exposure;
  foreach (i = 0 ... size) {
    reds[i] = saturating_add(reds[i], factor);
    greens[i] = saturating_add(greens[i], factor);
    blues[i] = saturating_add(blues[i], factor);
  }
}
