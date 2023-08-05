
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

#define RED_SHIFT 0
#define GREEN_SHIFT 8
#define BLUE_SHIFT 16
#define ALPHA_SHIFT 24

#define GET_RED(x) ((x >> RED_SHIFT) & 0xFF)
#define GET_GREEN(x) ((x >> GREEN_SHIFT) & 0xFF)
#define GET_BLUE(x) ((x >> BLUE_SHIFT) & 0xFF)
#define GET_ALPHA(x) ((x >> ALPHA_SHIFT) & 0xFF)

#define COLOR(r, g, b, a)                                      \
  ((r << RED_SHIFT) | (g << GREEN_SHIFT) | (b << BLUE_SHIFT) | \
   (a << ALPHA_SHIFT))

export void ToGrayscale(uniform uint32 buffer[], uniform uint64 size) {
  foreach (index = 0 ... size) {
    uint32 color = buffer[index];
    uint8 red = GET_RED(color);
    uint8 green = GET_GREEN(color);
    uint8 blue = GET_BLUE(color);
    uint8 alpha = GET_ALPHA(color);
    uint8 gray = (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
    buffer[index] = COLOR(gray, gray, gray, alpha);
  }
}
