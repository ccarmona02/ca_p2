#ifndef IMAGES_COMMON_BITMAP_HEADER_HPP
#define IMAGES_COMMON_BITMAP_HEADER_HPP

#include <iosfwd>
#include <tuple>
#include <cstdint>
#include <vector>
#include <array>

namespace images::common {

  class bitmap_header {
  public:
    bitmap_header() noexcept = default;
    bitmap_header(int w, int h) noexcept;

    bool operator==(const bitmap_header &) const noexcept = default;

    void read(std::istream & is);
    void write(std::ostream & os) const;

    void print_info(std::ostream & os) const noexcept;

    [[nodiscard]] int width() const noexcept { return width_; }

    [[nodiscard]] int height() const noexcept { return height_; }

    [[nodiscard]] long image_size() const noexcept {
      return static_cast<long>(width_) * static_cast<long>(height_);
    }

    friend void print_diff(const bitmap_header & h1, const bitmap_header & h2) noexcept;

  private:
    void read_buffer(std::istream & is);
    void write_buffer(std::ostream & os) const;

    static constexpr int header_size = 54;
    static constexpr int default_bit_count = 24;
    std::array<uint8_t, header_size> header_info{};
    std::vector<char> extra_buffer{};

    unsigned int pixel_start_ = 0;
    int width_ = 0;
    int height_ = 0;
    int planes_ = 1;
    int bit_count_ = default_bit_count;
    int compression_ = 0;
    int extra_size_ = 0;
  };

}

#endif // IMAGES_COMMON_BITMAP_HEADER_HPP