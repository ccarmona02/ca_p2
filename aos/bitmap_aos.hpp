#ifndef IMAGES_AOS_BITMAP_AOS_HPP
#define IMAGES_AOS_BITMAP_AOS_HPP

#include <filesystem>
#include <vector>
#include "common/bitmap_header.hpp"
#include "common/pixel.hpp"
#include "common/histogram.hpp"

namespace images::aos {

  using namespace images::common;

  class bitmap_aos {
  public:
    explicit bitmap_aos() noexcept = default;
    bitmap_aos(int w, int h);

    bool operator==(const bitmap_aos &) const noexcept = default;

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
    void set_pixel(int r, int c, common::pixel p) noexcept;

    friend void print_diff(const bitmap_aos & bm1, const bitmap_aos & bm2) noexcept;

  private:
    [[nodiscard]] int index(int r, int c) const noexcept;

    bitmap_header header{};
    std::vector<pixel> pixels;
  };

} // namespace images::aos

#endif // IMAGES_AOS_BITMAP_AOS_HPP
