#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <optional>

namespace ns {

using ScalarF = float;

struct UnitScalarF {
  constexpr UnitScalarF(ScalarF val) : value_(std::clamp(val, 0.0f, 1.0f)) {}

  constexpr operator ScalarF() const { return value_; }

 private:
  ScalarF value_ = 1.0f;
};

struct Matrix {
  union {
    ScalarF m[16];
    ScalarF e[4][4];
  };
  constexpr Matrix()
      : m{
            // clang-format off
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
            // clang-format on
        } {}

  constexpr Matrix(
      // clang-format off
    ScalarF m00, ScalarF m01, ScalarF m02, ScalarF m03, //
    ScalarF m10, ScalarF m11, ScalarF m12, ScalarF m13, //
    ScalarF m20, ScalarF m21, ScalarF m22, ScalarF m23, //
    ScalarF m30, ScalarF m31, ScalarF m32, ScalarF m33  //
                   // clang-format on
      )
      : m{
            // clang-format off
    m00, m01, m02, m03, //
    m10, m11, m12, m13, //
    m20, m21, m22, m23, //
    m30, m31, m32, m33  //
                 // clang-format on
        } {}
};

template <class T>
struct TPoint {
  using Type = T;

  Type x = {};
  Type y = {};

  constexpr TPoint() = default;

  constexpr TPoint(Type p_x, Type p_y) : x(p_x), y(p_y) {}

  constexpr bool operator==(const TPoint& other) const = default;

  constexpr bool operator!=(const TPoint& other) const = default;

  constexpr bool IsEmpty() const { return x < Type{} || y < Type{}; }

  constexpr Type GetArea() const { return x * y; }
};

template <class T>
struct TRect {
  TPoint<T> origin;
  TPoint<T> size;

  constexpr TRect() = default;

  constexpr TRect(TPoint<T> p_origin, TPoint<T> p_size)
      : origin(p_origin), size(p_size) {}

  constexpr TRect(T x, T y, T width, T height)
      : origin({x, y}), size({width, height}) {}

  constexpr TRect(TPoint<T> sz) : origin({0, 0}), size(sz.x, sz.y) {}

  constexpr std::array<T, 4> GetLTRB() const {
    const auto left = std::min(origin.x, origin.x + size.x);
    const auto top = std::min(origin.y, origin.y + size.y);
    const auto right = std::max(origin.x, origin.x + size.x);
    const auto bottom = std::max(origin.y, origin.y + size.y);
    return {left, top, right, bottom};
  }

  constexpr TPoint<T> GetLT() const {
    const auto left = std::min(origin.x, origin.x + size.width);
    const auto top = std::min(origin.y, origin.y + size.height);
    return {left, top};
  }

  constexpr TPoint<T> GetRB() const {
    const auto right = std::max(origin.x, origin.x + size.width);
    const auto bottom = std::max(origin.y, origin.y + size.height);
    return {right, bottom};
  }

  constexpr static TRect MakeLTRB(T left, T top, T right, T bottom) {
    return TRect(left, top, right - left, bottom - top);
  }

  constexpr std::optional<TRect> Intersection(const TRect& o) const {
    auto this_ltrb = GetLTRB();
    auto other_ltrb = o.GetLTRB();
    auto intersection =
        TRect::MakeLTRB(std::max(this_ltrb[0], other_ltrb[0]),  //
                        std::max(this_ltrb[1], other_ltrb[1]),  //
                        std::min(this_ltrb[2], other_ltrb[2]),  //
                        std::min(this_ltrb[3], other_ltrb[3])   //
        );
    if (intersection.size.IsEmpty()) {
      return std::nullopt;
    }
    return intersection;
  }

  constexpr bool operator==(const TRect& other) const = default;

  constexpr bool operator!=(const TRect& other) const = default;
};

using Point = TPoint<int32_t>;
using UPoint = TPoint<uint32_t>;
using Rect = TRect<uint32_t>;

struct Radians {
  ScalarF radians = 0.0;

  constexpr Radians() = default;

  explicit constexpr Radians(ScalarF p_radians) : radians(p_radians) {}
};

struct Degrees {
  ScalarF degrees = 0.0;

  constexpr Degrees() = default;

  explicit constexpr Degrees(ScalarF p_degrees)
      : degrees(fmodf(p_degrees, 360.0f)) {}

  constexpr operator Radians() const {
    return Radians(degrees * M_PI / 180.0f);
  };
};

struct Color {
  union {
    uint32_t color = {};
    struct {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t alpha;
    };
  };

  constexpr Color() = default;

  constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
      : red(r), green(g), blue(b), alpha(a) {}

  constexpr Color(uint32_t p_color) : color(p_color) {}

  constexpr bool operator==(const Color& other) const {
    return color == other.color;
  }

  constexpr bool operator!=(const Color& other) const {
    return !(*this == other);
  }

  constexpr operator uint32_t() const { return color; }

  constexpr Color WithRed(uint8_t color) const {
    return Color(color, green, blue, alpha);
  }

  constexpr Color WithGreen(uint8_t color) const {
    return Color(red, color, blue, alpha);
  }

  constexpr Color WithBlue(uint8_t color) const {
    return Color(red, green, color, alpha);
  }

  constexpr Color WithAlpha(uint8_t color) const {
    return Color(red, green, blue, color);
  }

  static constexpr Color FromComponentsF(ScalarF r,
                                         ScalarF g,
                                         ScalarF b,
                                         ScalarF a) {
    return Color{
        static_cast<uint8_t>(255 * r),
        static_cast<uint8_t>(255 * g),
        static_cast<uint8_t>(255 * b),
        static_cast<uint8_t>(255 * a),
    };
  }

  static Color Random() {
    return Color{static_cast<uint8_t>(std::rand() % 255),
                 static_cast<uint8_t>(std::rand() % 255),
                 static_cast<uint8_t>(std::rand() % 255), 255};
  }

  static constexpr Color Gray(ScalarF gray) {
    const uint8_t g = 255 * gray;
    return Color{g, g, g, 255};
  }
};

constexpr Color kColorRed = {255, 0, 0, 255};
constexpr Color kColorGreen = {0, 255, 0, 255};
constexpr Color kColorBlue = {0, 0, 255, 255};
constexpr Color kColorWhite = {255, 255, 255, 255};
constexpr Color kColorBlack = {0, 0, 0, 255};
constexpr Color kColorTransparentBlack = {0, 0, 0, 0};
constexpr Color kColorAliceBlue = {240, 248, 255, 255};
constexpr Color kColorAntiqueWhite = {250, 235, 215, 255};
constexpr Color kColorAqua = {0, 255, 255, 255};
constexpr Color kColorAquaMarine = {127, 255, 212, 255};
constexpr Color kColorAzure = {240, 255, 255, 255};
constexpr Color kColorBeige = {245, 245, 220, 255};
constexpr Color kColorBisque = {255, 228, 196, 255};
constexpr Color kColorBlanchedAlmond = {255, 235, 205, 255};
constexpr Color kColorBlueViolet = {138, 43, 226, 255};
constexpr Color kColorBrown = {165, 42, 42, 255};
constexpr Color kColorBurlyWood = {222, 184, 135, 255};
constexpr Color kColorCadetBlue = {95, 158, 160, 255};
constexpr Color kColorChartreuse = {127, 255, 0, 255};
constexpr Color kColorChocolate = {210, 105, 30, 255};
constexpr Color kColorCoral = {255, 127, 80, 255};
constexpr Color kColorCornflowerBlue = {100, 149, 237, 255};
constexpr Color kColorCornsilk = {255, 248, 220, 255};
constexpr Color kColorCrimson = {220, 20, 60, 255};
constexpr Color kColorCyan = {0, 255, 255, 255};
constexpr Color kColorDarkBlue = {0, 0, 139, 255};
constexpr Color kColorDarkCyan = {0, 139, 139, 255};
constexpr Color kColorDarkGoldenrod = {184, 134, 11, 255};
constexpr Color kColorDarkGray = {169, 169, 169, 255};
constexpr Color kColorDarkGreen = {0, 100, 0, 255};
constexpr Color kColorDarkGrey = {169, 169, 169, 255};
constexpr Color kColorDarkKhaki = {189, 183, 107, 255};
constexpr Color kColorDarkMagenta = {139, 0, 139, 255};
constexpr Color kColorDarkOliveGreen = {85, 107, 47, 255};
constexpr Color kColorDarkOrange = {255, 140, 0, 255};
constexpr Color kColorDarkOrchid = {153, 50, 204, 255};
constexpr Color kColorDarkRed = {139, 0, 0, 255};
constexpr Color kColorDarkSalmon = {233, 150, 122, 255};
constexpr Color kColorDarkSeagreen = {143, 188, 143, 255};
constexpr Color kColorDarkSlateBlue = {72, 61, 139, 255};
constexpr Color kColorDarkSlateGray = {47, 79, 79, 255};
constexpr Color kColorDarkSlateGrey = {47, 79, 79, 255};
constexpr Color kColorDarkTurquoise = {0, 206, 209, 255};
constexpr Color kColorDarkViolet = {148, 0, 211, 255};
constexpr Color kColorDeepPink = {255, 20, 147, 255};
constexpr Color kColorDeepSkyBlue = {0, 191, 255, 255};
constexpr Color kColorDimGray = {105, 105, 105, 255};
constexpr Color kColorDimGrey = {105, 105, 105, 255};
constexpr Color kColorDodgerBlue = {30, 144, 255, 255};
constexpr Color kColorFirebrick = {178, 34, 34, 255};
constexpr Color kColorFloralWhite = {255, 250, 240, 255};
constexpr Color kColorForestGreen = {34, 139, 34, 255};
constexpr Color kColorFuchsia = {255, 0, 255, 255};
constexpr Color kColorGainsboro = {220, 220, 220, 255};
constexpr Color kColorGhostwhite = {248, 248, 255, 255};
constexpr Color kColorGold = {255, 215, 0, 255};
constexpr Color kColorGoldenrod = {218, 165, 32, 255};
constexpr Color kColorGray = {128, 128, 128, 255};
constexpr Color kColorGreenYellow = {173, 255, 47, 255};
constexpr Color kColorGrey = {128, 128, 128, 255};
constexpr Color kColorHoneydew = {240, 255, 240, 255};
constexpr Color kColorHotPink = {255, 105, 180, 255};
constexpr Color kColorIndianRed = {205, 92, 92, 255};
constexpr Color kColorIndigo = {75, 0, 130, 255};
constexpr Color kColorIvory = {255, 255, 240, 255};
constexpr Color kColorKhaki = {240, 230, 140, 255};
constexpr Color kColorLavender = {230, 230, 250, 255};
constexpr Color kColorLavenderBlush = {255, 240, 245, 255};
constexpr Color kColorLawnGreen = {124, 252, 0, 255};
constexpr Color kColorLemonChiffon = {255, 250, 205, 255};
constexpr Color kColorLightBlue = {173, 216, 230, 255};
constexpr Color kColorLightCoral = {240, 128, 128, 255};
constexpr Color kColorLightCyan = {224, 255, 255, 255};
constexpr Color kColorLightGoldenrodYellow = {50, 250, 210, 255};
constexpr Color kColorLightGray = {211, 211, 211, 255};
constexpr Color kColorLightGreen = {144, 238, 144, 255};
constexpr Color kColorLightGrey = {211, 211, 211, 255};
constexpr Color kColorLightPink = {255, 182, 193, 255};
constexpr Color kColorLightSalmon = {255, 160, 122, 255};
constexpr Color kColorLightSeaGreen = {32, 178, 170, 255};
constexpr Color kColorLightSkyBlue = {135, 206, 250, 255};
constexpr Color kColorLightSlateGray = {119, 136, 153, 255};
constexpr Color kColorLightSlateGrey = {119, 136, 153, 255};
constexpr Color kColorLightSteelBlue = {176, 196, 222, 255};
constexpr Color kColorLightYellow = {255, 255, 224, 255};
constexpr Color kColorLime = {0, 255, 0, 255};
constexpr Color kColorLimeGreen = {50, 205, 50, 255};
constexpr Color kColorLinen = {250, 240, 230, 255};
constexpr Color kColorMagenta = {255, 0, 255, 255};
constexpr Color kColorMaroon = {128, 0, 0, 255};
constexpr Color kColorMediumAquamarine = {102, 205, 170, 255};
constexpr Color kColorMediumBlue = {0, 0, 205, 255};
constexpr Color kColorMediumOrchid = {186, 85, 211, 255};
constexpr Color kColorMediumPurple = {147, 112, 219, 255};
constexpr Color kColorMediumSeagreen = {60, 179, 113, 255};
constexpr Color kColorMediumSlateBlue = {123, 104, 238, 255};
constexpr Color kColorMediumSpringGreen = {0, 250, 154, 255};
constexpr Color kColorMediumTurquoise = {72, 209, 204, 255};
constexpr Color kColorMediumVioletRed = {199, 21, 133, 255};
constexpr Color kColorMidnightBlue = {25, 25, 112, 255};
constexpr Color kColorMintCream = {245, 255, 250, 255};
constexpr Color kColorMistyRose = {255, 228, 225, 255};
constexpr Color kColorMoccasin = {255, 228, 181, 255};
constexpr Color kColorNavajoWhite = {255, 222, 173, 255};
constexpr Color kColorNavy = {0, 0, 128, 255};
constexpr Color kColorOldLace = {253, 245, 230, 255};
constexpr Color kColorOlive = {128, 128, 0, 255};
constexpr Color kColorOliveDrab = {107, 142, 35, 255};
constexpr Color kColorOrange = {255, 165, 0, 255};
constexpr Color kColorOrangeRed = {255, 69, 0, 255};
constexpr Color kColorOrchid = {218, 112, 214, 255};
constexpr Color kColorPaleGoldenrod = {238, 232, 170, 255};
constexpr Color kColorPaleGreen = {152, 251, 152, 255};
constexpr Color kColorPaleTurquoise = {175, 238, 238, 255};
constexpr Color kColorPaleVioletRed = {219, 112, 147, 255};
constexpr Color kColorPapayaWhip = {255, 239, 213, 255};
constexpr Color kColorPeachpuff = {255, 218, 185, 255};
constexpr Color kColorPeru = {205, 133, 63, 255};
constexpr Color kColorPink = {255, 192, 203, 255};
constexpr Color kColorPlum = {221, 160, 221, 255};
constexpr Color kColorPowderBlue = {176, 224, 230, 255};
constexpr Color kColorPurple = {128, 0, 128, 255};
constexpr Color kColorRosyBrown = {188, 143, 143, 255};
constexpr Color kColorRoyalBlue = {65, 105, 225, 255};
constexpr Color kColorSaddleBrown = {139, 69, 19, 255};
constexpr Color kColorSalmon = {250, 128, 114, 255};
constexpr Color kColorSandyBrown = {244, 164, 96, 255};
constexpr Color kColorSeagreen = {46, 139, 87, 255};
constexpr Color kColorSeashell = {255, 245, 238, 255};
constexpr Color kColorSienna = {160, 82, 45, 255};
constexpr Color kColorSilver = {192, 192, 192, 255};
constexpr Color kColorSkyBlue = {135, 206, 235, 255};
constexpr Color kColorSlateBlue = {106, 90, 205, 255};
constexpr Color kColorSlateGray = {112, 128, 144, 255};
constexpr Color kColorSlateGrey = {112, 128, 144, 255};
constexpr Color kColorSnow = {255, 250, 250, 255};
constexpr Color kColorSpringGreen = {0, 255, 127, 255};
constexpr Color kColorSteelBlue = {70, 130, 180, 255};
constexpr Color kColorTan = {210, 180, 140, 255};
constexpr Color kColorTeal = {0, 128, 128, 255};
constexpr Color kColorThistle = {216, 191, 216, 255};
constexpr Color kColorTomato = {255, 99, 71, 255};
constexpr Color kColorTurquoise = {64, 224, 208, 255};
constexpr Color kColorViolet = {238, 130, 238, 255};
constexpr Color kColorWheat = {245, 222, 179, 255};
constexpr Color kColorWhitesmoke = {245, 245, 245, 255};
constexpr Color kColorYellow = {255, 255, 0, 255};
constexpr Color kColorYellowGreen = {154, 205, 50, 255};

static_assert(sizeof(kColorRed) == sizeof(uint32_t));

}  // namespace ns
