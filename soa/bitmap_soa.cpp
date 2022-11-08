#include "bitmap_soa.hpp"
#include "common/file_error.hpp"
#include <fstream>

namespace images::soa {

  bitmap_soa::bitmap_soa(int w, int h) : header{w, h},
                                         pixels{std::vector<uint8_t>(header.image_size()),
                                                std::vector<uint8_t>(header.image_size()),
                                                std::vector<uint8_t>(header.image_size())} {
  }

  void bitmap_soa::read(const std::filesystem::path & in_name) {
    std::ifstream in{in_name};
    if (!in) {
      throw file_error{file_error_kind::cannot_open};
    }
    header.read(in);

    const int extra = (width() * 3) % 4;
    const int pixels_size = height() * width();
    for (auto & p: pixels) {
      p.reserve(pixels_size);
    }
    for (int r = 0; r < height(); ++r) {
      for (int c = 0; c < width(); ++c) {
        pixel pixel_value{};
        pixel_value.read(in);
        pixels[red_channel].push_back(pixel_value.red());
        pixels[green_channel].push_back(pixel_value.green());
        pixels[blue_channel].push_back(pixel_value.blue());
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

  void bitmap_soa::write(const std::filesystem::path & out_name) {
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

  void bitmap_soa::to_gray() noexcept {
    const auto max = header.image_size();
    for (long i = 0; i < max; ++i) {
      const auto gray_level = to_gray_corrected(pixels[red_channel][i], pixels[green_channel][i],
          pixels[blue_channel][i]);
      pixels[red_channel][i] = gray_level;
      pixels[green_channel][i] = gray_level;
      pixels[blue_channel][i] = gray_level;
    }
  }

  bool bitmap_soa::is_gray() const noexcept {
    const auto max = std::ssize(pixels);
    for (int i = 0; i < max; ++i) {
      const auto p = get_pixel(i);
      if (!p.is_gray()) { return false; }
    }
    return true;
  }

  namespace {
    constexpr std::array<int, 25> gauss_kernel{1, 4, 7, 4, 1, 4, 16, 26, 16, 4, 7, 26, 41, 26, 7, 4,
                                               16, 26, 16, 4, 1, 4, 7, 4, 1};

    constexpr int gauss_norm = 273;
    constexpr auto gauss_size = std::ssize(gauss_kernel);
  }

  void bitmap_soa::gauss() noexcept {
    bitmap_soa result{*this};
    const auto num_pixels = std::ssize(pixels[red_channel]);
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
        accum += get_pixel(gauss_pixel_index) * gauss_value;
      }
      result.set_pixel(pixel_index, pixel{accum / gauss_norm});
    }
    *this = result;
  }

  histogram bitmap_soa::generate_histogram() const noexcept {
    histogram histo;
    const int pixel_count = width() * height();
    for (int i = 0; i < pixel_count; ++i) {
      histo.add_red(pixels[red_channel][i]);
      histo.add_green(pixels[green_channel][i]);
      histo.add_blue(pixels[blue_channel][i]);
    }
    return histo;
  }

  void bitmap_soa::print_info(std::ostream & os) const noexcept {
    header.print_info(os);
  }

  pixel bitmap_soa::get_pixel(int r, int c) const noexcept {
    auto i = index(r, c);
    return get_pixel(i);
  }

  void bitmap_soa::set_pixel(int r, int c, pixel p) noexcept {
    auto i = index(r, c);
    set_pixel(i, p);
  }

  pixel bitmap_soa::get_pixel(int i) const noexcept {
    pixel p{pixels[red_channel][i], pixels[green_channel][i], pixels[blue_channel][i]};
    return p;
  }

  void bitmap_soa::set_pixel(int i, pixel p) noexcept {
    pixels[red_channel][i] = p.red();
    pixels[green_channel][i] = p.green();
    pixels[blue_channel][i] = p.blue();
  }

  int bitmap_soa::index(int r, int c) const noexcept {
    return r * width() + c;
  }

} // namespace images::soa
