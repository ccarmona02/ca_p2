#ifndef IMAGES_SOA_BITMAP_SOA_HPP
#define IMAGES_SOA_BITMAP_SOA_HPP

#include <filesystem>
#include <vector>
#include "common/bitmap_header.hpp"
#include "common/pixel.hpp"
#include "common/histogram.hpp"

namespace images::soa {

  using namespace images::common;

  class bitmap_soa {
  public:
    explicit bitmap_soa() noexcept = default;
    bitmap_soa(int w, int h);

    bool operator==(const bitmap_soa &) const noexcept = default;

    void read(const std::filesystem::path & in_name);
    void write(const std::filesystem::path & out_name);

    void to_gray() noexcept;
    void gauss() noexcept;
    [[nodiscard]] histogram generate_histogram() const noexcept;
    void print_info(std::ostream & os) const noexcept;

    [[nodiscard]] int width() const noexcept { return header.width(); }

    [[nodiscard]] int height() const noexcept { return header.height(); }

    [[nodiscard]] auto get_size() const {
      return std::tuple{width(), height()};
    }

    [[nodiscard]] auto get_pixel_position(int index) const {
      return std::tuple{index / width(), index % width()};
    }

    [[nodiscard]] bool is_gray() const noexcept;

    [[nodiscard]] pixel get_pixel(int r, int c) const noexcept;
    void set_pixel(int r, int c, pixel p) noexcept;

  private:
    [[nodiscard]] int index(int r, int c) const noexcept;
    [[nodiscard]] pixel get_pixel(int i) const noexcept;
    void set_pixel(int i, pixel p) noexcept;

    bitmap_header header{};
    std::array<std::vector<uint8_t>, num_channels> pixels;
  };

} // namespace images::soa

#endif // IMAGES_SOA_BITMAP_SOA_HPP
