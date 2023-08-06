
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

