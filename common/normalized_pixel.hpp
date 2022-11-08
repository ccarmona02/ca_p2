#ifndef IMAGES_COMMON_NORMALIZED_PIXEL_HPP
#define IMAGES_COMMON_NORMALIZED_PIXEL_HPP

#include "common/pixel.hpp"

namespace images::common {

  static constexpr double intensity_max_value = 255.0;

  class normalized_pixel {
  public:
    normalized_pixel(uint8_t r, uint8_t g, uint8_t b) noexcept: color{b / intensity_max_value,
                                                                      g / intensity_max_value,
                                                                      r / intensity_max_value} { }

    [[nodiscard]] double red() const noexcept { return color[red_channel]; }

    [[nodiscard]] double green() const noexcept { return color[green_channel]; }

    [[nodiscard]] double blue() const noexcept { return color[blue_channel]; }

    void intensity_transform() noexcept;

    [[nodiscard]] double to_gray() const noexcept;

  private:
    std::array<double, num_channels> color;
  };

  double gamma_correction(double g) noexcept;
  uint8_t gray_denormalize(double g) noexcept;

}

#endif //IMAGES_COMMON_NORMALIZED_PIXEL_HPP
