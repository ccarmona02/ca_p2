#include "bitmap_aos.hpp"
#include "common/file_error.hpp"
#include <fstream>

namespace images::aos {

  bitmap_aos::bitmap_aos(int w, int h) : header{w, h}, pixels(static_cast<std::size_t>(w * h)) {
  }

  void bitmap_aos::read(const std::filesystem::path & in_name) {
    using namespace images::common;
    std::ifstream in{in_name};
    if (!in) {
      throw file_error{file_error_kind::cannot_open};
    }
    header.read(in);

    const int extra = (width() * 3) % 4;
    const int pixels_size = height() * width();
    pixels.reserve(pixels_size);
    for (int r = 0; r < height(); ++r) {
      for (int c = 0; c < width(); ++c) {
        pixel pixel_value{};
        pixel_value.read(in);
        pixels.push_back(pixel_value);
      }
      if (extra != 0) {
        in.ignore(4 - extra);
      }
    }

  }

  namespace {
    void write_padding(std::ostream & os, int n) noexcept {
      std::array<uint8_t, 3> pad_pixel{};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      os.write(reinterpret_cast<char *>(pad_pixel.data()), n);
    }
  }

  void bitmap_aos::write(const std::filesystem::path & out_name) {
    using namespace images::common;
    std::ofstream out{out_name};
    if (!out) {
      throw file_error{file_error_kind::cannot_open};
    }

    header.write(out);
    const int padding = (4 - (width() * 3) % 4) % 4;
    for (int r = 0; r < height(); ++r) {
      for (int c = 0; c < width(); ++c) {
        auto pixel_value = get_pixel(r, c);
        pixel_value.write(out);
      }
      write_padding(out, padding);
    }
  }

  void bitmap_aos::to_gray() noexcept {
    const auto max = std::ssize(pixels);
    for (int i = 0; i < max; ++i) {
      pixels[i] = pixels[i].to_gray_corrected();
    }
  }

  bool bitmap_aos::is_gray() const noexcept {
    const auto max = std::ssize(pixels);
    for (int i = 0; i < max; ++i) {
      if (!pixels[i].is_gray()) { return false; }
    }
    return true;
  }

  namespace {
    constexpr std::array<int, 25> gauss_kernel{1, 4, 7, 4, 1, 4, 16, 26, 16, 4, 7, 26, 41, 26, 7, 4,
                                               16, 26, 16, 4, 1, 4, 7, 4, 1};

    constexpr int gauss_norm = 273;
    constexpr auto gauss_size = std::ssize(gauss_kernel);
  }

  void bitmap_aos::gauss() noexcept {
    bitmap_aos result{*this};
    const auto num_pixels = std::ssize(pixels);
    const auto [pixels_width, pixels_height] = get_size();
    for (int pixel_index = 0; pixel_index < num_pixels; ++pixel_index) {
      const auto [row, column] = get_pixel_position(pixel_index);
      color_accumulator accum;
      for (int gauss_index = 0; gauss_index < gauss_size; ++gauss_index) {
        const int column_offset = (gauss_index % 5) - 2;
        const int j = column + column_offset;
        if (j < 0 || j >= pixels_width) { continue; }
        const int row_offset = (gauss_index / 5) - 2;
        const int i = row + row_offset;
        if (i < 0 || i >= pixels_height) { continue; }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        const int gauss_value = gauss_kernel[gauss_index];
        const auto gauss_pixel_index = index(i, j);
        accum += pixels[gauss_pixel_index] * gauss_value;
      }
      result.pixels[pixel_index] = accum / gauss_norm;
    }
    *this = result;
  }

  histogram bitmap_aos::generate_histogram() const noexcept {
    histogram histo;
    const int pixel_count = width() * height();
    for (int i = 0; i < pixel_count; ++i) {
      histo.add_color(pixels[i]);
    }
    return histo;
  }

  void bitmap_aos::print_info(std::ostream & os) const noexcept {
    header.print_info(os);
  }

  pixel bitmap_aos::get_pixel(int r, int c) const noexcept {
    return pixels[index(r, c)];
  }

  void bitmap_aos::set_pixel(int r, int c, pixel p) noexcept {
    pixels[index(r, c)] = p;
  }

  int bitmap_aos::index(int r, int c) const noexcept {
    return r * width() + c;
  }

  void print_diff(const bitmap_aos & bm1, const bitmap_aos & bm2) noexcept {
    std::cout << "Printing differences:\n";
    print_diff(bm1.header, bm2.header);
    const auto num_pixels = std::ssize(bm1.pixels);
    for (int i = 0; i < num_pixels; ++i) {
      if (bm1.pixels[i] != bm2.pixels[i]) {
        std::cout << "  Pixel " << i << " is different";
        std::cout << bm1.pixels[i] << " -- " << bm2.pixels[i] << "\n";
        return;
      }
    }
    std::cout << "All pixels are equal\n";
  }


} // namespace images::aos
