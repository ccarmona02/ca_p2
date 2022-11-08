#ifndef IMAGES_COMMON_PIXEL_HPP
#define IMAGES_COMMON_PIXEL_HPP

#include <cstdint>
#include <array>
#include <iostream>

namespace images::common {

  static constexpr int num_channels = 3;
  static constexpr int red_channel = 2;
  static constexpr int green_channel = 1;
  static constexpr int blue_channel = 0;

  class color_accumulator {
  public:
    color_accumulator() = default;

    color_accumulator(long r, long g, long b) : color{b, g, r} { }

    /*
    void add_multiply(int coefficient, pixel p) {
      color[red_channel] += static_cast<long>(coefficient * p.red());
      color[green_channel] += static_cast<long>(coefficient * p.green());
      color[blue_channel] += static_cast<long>(coefficient * p.blue());
    }
     */
    color_accumulator & operator+=(const color_accumulator & c) {
      color[red_channel] += c.color[red_channel];
      color[green_channel] += c.color[green_channel];
      color[blue_channel] += c.color[blue_channel];
      return *this;
    }

    color_accumulator operator/(int factor) {
      color_accumulator result(*this);
      for (auto & c: result.color) {
        c /= factor;
      }
      return result;
    }

    [[nodiscard]] long red() const noexcept { return color[red_channel]; }

    [[nodiscard]] long green() const noexcept { return color[green_channel]; }

    [[nodiscard]] long blue() const noexcept { return color[blue_channel]; }

  private:
    std::array<long, num_channels> color{};
  };

  // Pixels in the 0..255 scale
  class pixel {
  public:
    pixel() noexcept = default;

    pixel(uint8_t r, uint8_t g, uint8_t b) noexcept: color{b, g, r} { }

    pixel(const color_accumulator & c) : pixel{static_cast<uint8_t>(c.red()),
                                               static_cast<uint8_t>(c.green()),
                                               static_cast<uint8_t>(c.blue())} { }

    bool operator==(const pixel &) const noexcept = default;

    pixel & operator=(const color_accumulator & c) noexcept;

    color_accumulator operator*(int coefficient) const {
      return color_accumulator{static_cast<long>(coefficient * red()),
                               static_cast<long>(coefficient * green()),
                               static_cast<long>(coefficient * blue())};
    }

    void read(std::istream & is) noexcept;
    void write(std::ostream & os) const noexcept;

    [[nodiscard]] uint8_t red() const noexcept { return color[red_channel]; }

    void red(uint8_t r) noexcept { color[red_channel] = r; }

    [[nodiscard]] uint8_t green() const noexcept { return color[green_channel]; }

    void green(uint8_t g) noexcept { color[green_channel] = g; }

    [[nodiscard]] uint8_t blue() const noexcept { return color[blue_channel]; }

    void blue(uint8_t b) noexcept { color[blue_channel] = b; }

    [[nodiscard]] pixel to_gray_corrected() const noexcept;
    [[nodiscard]] bool is_gray() const noexcept;

    friend std::ostream & operator<<(std::ostream & os, pixel p) {
      os << "(" << static_cast<int>(p.red()) << "," << static_cast<int>(p.green()) << ","
         << static_cast<int>(p.blue()) << ")";
      return os;
    }

  private:
    std::array<uint8_t, num_channels> color;
  };

  uint8_t to_gray_corrected(uint8_t r, uint8_t g, uint8_t b) noexcept;


  inline pixel & pixel::operator=(const color_accumulator & c) noexcept {
    color[red_channel] = c.red();
    color[green_channel] = c.green();
    color[blue_channel] = c.blue();
    return *this;
  }

} // images::common

#endif //IMAGES_COMMON_PIXEL_HPP
