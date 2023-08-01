
export void Clear(uniform uint32 buffer[],
                  uniform uint64 size,
                  uniform uint32 color) {
  foreach (index = 0 ... size) {
    buffer[index] = color;
  }
}

struct Color {
  uint8 alpha;
  uint8 red;
  uint8 green;
  uint8 blue;
};

export void ToGrayscale(uniform Color buffer[], uniform uint64 size) {
  foreach (index = 0 ... size) {
    Color color = buffer[index];
    uint8 gray =
        (0.2126 * color.red) + (0.7152 * color.green) + (0.0722 * color.blue);
    buffer[index].red = gray;
    buffer[index].green = gray;
    buffer[index].blue = gray;
  }
}
