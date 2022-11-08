#include "pixel.hpp"
#include "normalized_pixel.hpp"

namespace images::common {

  pixel pixel::to_gray_corrected() const noexcept {
    const auto gray_level = images::common::to_gray_corrected(red(), green(), blue());
    return {gray_level, gray_level, gray_level};
  }

  uint8_t to_gray_corrected(uint8_t r, uint8_t g, uint8_t b) noexcept {
    auto real_pixel = normalized_pixel(r, g, b);
    real_pixel.intensity_transform();
    const double gray = real_pixel.to_gray();
    const double gray_gamma = gamma_correction(gray);
    return gray_denormalize(gray_gamma);
  }

  void pixel::read(std::istream & is) noexcept {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    is.read(reinterpret_cast<char *>(color.data()), std::ssize(color));
  }

  void pixel::write(std::ostream & os) const noexcept {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    os.write(reinterpret_cast<const char *>(color.data()), std::ssize(color));
  }

  bool pixel::is_gray() const noexcept {
    return (color[red_channel] == color[green_channel] and
            color[red_channel] == color[blue_channel]);
  }

}
