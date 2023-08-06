
struct Color {
  uint8 red;
  uint8 green;
  uint8 blue;
  uint8 alpha;
};

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
    uint8 red = reds[i];
    uint8 green = greens[i];
    uint8 blue = blues[i];
    uint8 gray = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
    reds[i] = gray;
    greens[i] = gray;
    blues[i] = gray;
  }
}
