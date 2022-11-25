#include "normalized_pixel.hpp"
#include <cmath>
#include <omp.h>

namespace images::common {

  namespace {
  [[maybe_unused]] constexpr double intensity_threshold = 0.04045;
    constexpr double intensity_divisor1 = 12.92;
    constexpr double intensity_divisor2 = 1.055;
    constexpr double intensity_delta = 0.055;
    constexpr double intensity_exponent = 2.4;
    constexpr double intensity_exponent_inv = 1.0 / intensity_exponent;
    constexpr double gamma_threshold = 0.003108;

    constexpr double red_coefficient = 0.2126;
    constexpr double green_coefficient = 0.7152;
    constexpr double blue_coefficient = 0.0722;
  }

  void normalized_pixel::intensity_transform() noexcept {
    for (int i = 0; i < static_cast<int>(color.size()); ++i) {
      if (color[i] <= intensity_threshold) {
        color[i] /= intensity_divisor1;
      }
      else {
        color[i] = std::pow((color[i] + intensity_delta) / intensity_divisor2, intensity_exponent);
      }
    }
  }

  double normalized_pixel::to_gray() const noexcept {
    return red_coefficient * red() + green_coefficient * green() + blue_coefficient * blue();
  }

  double gamma_correction(double g) noexcept {
    if (g <= gamma_threshold) {
      return intensity_divisor1 * g;
    }
    return intensity_divisor2 * std::pow(g, intensity_exponent_inv) - intensity_delta;
  }

  uint8_t gray_denormalize(double g) noexcept {
    return static_cast<uint8_t>(g * intensity_max_value);
  }

}