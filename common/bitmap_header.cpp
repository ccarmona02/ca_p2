#include "bitmap_header.hpp"
#include "file_error.hpp"

#include <iostream>
#include <span>

namespace {
  constexpr int pixel_start_offset = 10;
  constexpr int width_offset = 18;
  constexpr int header_offset = 22;
  constexpr int planes_offset = 26;
  constexpr int bit_count_offset = 28;
  constexpr int compression_offset = 30;

  constexpr int expected_bit_count = 24;

  template<typename T, typename S>
  T get_value(S buffer, int offset) noexcept {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *reinterpret_cast<T *>(buffer.data() + offset);
  }

  template<typename T, typename S>
  void set_value(T value, S buffer, int offset) noexcept {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    T *p = reinterpret_cast<T *>(buffer.data() + offset);
    *p = value;
  }

  void check_file_error(bool pred, images::common::file_error_kind error_kind) {
    if (!pred) {
      throw images::common::file_error{error_kind};
    }
  }

}

namespace images::common {

  bitmap_header::bitmap_header(int w, int h) noexcept: pixel_start_{header_size}, width_{w},
                                                       height_{h} {
    const std::span header_view{header_info};
    set_value(pixel_start_, header_view, pixel_start_offset);
    set_value(width_, header_view, width_offset);
    set_value(height_, header_view, header_offset);
    set_value(planes_, header_view, planes_offset);
    set_value(bit_count_, header_view, bit_count_offset);
    set_value(compression_, header_view, compression_offset);
  }

  void bitmap_header::read_buffer(std::istream & is) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    is.read(reinterpret_cast<char *>(header_info.data()), header_size);
    if (!is) {
      throw file_error{file_error_kind::cannot_read};
    }
  }

  void bitmap_header::write_buffer(std::ostream & os) const {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    os.write(reinterpret_cast<const char *>(header_info.data()), header_size);
    if (!os) {
      throw file_error{file_error_kind::cannot_write};
    }
  }

  void bitmap_header::print_info(std::ostream & os) const noexcept {
    os << "    Width: " << width_ << '\n';
    os << "    Height: " << height_ << '\n';
    os << "    Pixel start: " << pixel_start_ << '\n';
    os << "    Number of planes: " << planes_ << '\n';
    os << "    Bits per pixel: " << bit_count_ << '\n';
    os << "    Compression: " << compression_ << '\n';
    os << "    Extra header size: " << extra_size_ << '\n';
  }

  void bitmap_header::read(std::istream & is) {
    using
    enum file_error_kind;
    read_buffer(is);
    const std::span header_view{header_info};
    check_file_error(header_view[0] == 'B' and header_view[1] == 'M', invalid_magic_number);

    pixel_start_ = get_value<int32_t>(header_view, pixel_start_offset);
    width_ = get_value<int32_t>(header_view, width_offset);
    height_ = get_value<int32_t>(header_view, header_offset);
    planes_ = get_value<int16_t>(header_view, planes_offset);
    bit_count_ = get_value<uint16_t>(header_view, bit_count_offset);
    compression_ = get_value<int32_t>(header_view, compression_offset);

    check_file_error(planes_ == 1, invalid_planes);
    check_file_error(bit_count_ == expected_bit_count, invlaid_bit_count);
    check_file_error(compression_ == 0, invalid_compression);

    extra_size_ = static_cast<int>(pixel_start_) - header_size;
    check_file_error(extra_size_ >= 0, invalid_pixel_start);
    extra_buffer.resize(extra_size_);
    is.read(extra_buffer.data(), extra_size_);
    check_file_error(static_cast<bool>(is), cannot_read_extra);
  }

  void bitmap_header::write(std::ostream & os) const {
    write_buffer(os);
    os.write(extra_buffer.data(), std::ssize(extra_buffer));
  }

  void print_diff(const bitmap_header & h1, const bitmap_header & h2) noexcept {
    std::cout << "Header differences:";
    if (h1 == h2) {
      std::cout << "  No differences\n";
    }
    else {
      std::cout << " There ae differences\n";
    }
  }

} // namespace images::common