#include <gtest/gtest.h>
#include "common/bitmap_header.hpp"
#include "common/file_error.hpp"
#include <span>

TEST(bitmap_heaer, read_invalid_size) {
  std::array<char, 10> buffer{};
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::istringstream in{buffer_string};
  images::common::bitmap_header header;
  EXPECT_THROW(header.read(in), images::common::file_error);
}

TEST(bitmap_header, read_invalid_magic) {
  std::array<char, 54> buffer{};
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  EXPECT_THROW(header.read(in), images::common::file_error);
}

TEST(bitmap_header, read_invalid_planes) {
  std::array<char, 54> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[26] = 2; // Planes
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  EXPECT_THROW(header.read(in), images::common::file_error);
}

TEST(bitmap_header, read_invalid_bit_count) {
  std::array<char, 54> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  EXPECT_THROW(header.read(in), images::common::file_error);
}

TEST(bitmap_header, read_invalid_compression) {
  std::array<char, 54> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  buffer[30] = 1; // Compression
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  EXPECT_THROW(header.read(in), images::common::file_error);
}

TEST(bitmap_header, read_pixel_start_zero) {
  std::array<char, 54> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[10] = 54; // Pixels start
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  buffer[30] = 0; // Compression
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  header.read(in);
}

TEST(bitmap_header, read_pixel_start_positive) {
  std::array<char, 130> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[10] = 100; // Pixels start
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  buffer[30] = 0; // Compression
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  header.read(in);
}

TEST(bitmap_header, read_pixel_start_negative) {
  std::array<char, 54> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[10] = 50; // Pixels start
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  buffer[30] = 0; // Compression
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  EXPECT_THROW(header.read(in), images::common::file_error);
}

TEST(bitmap_header, write) {
  std::array<char, 100> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[10] = 100; // Pixels start
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  buffer[30] = 0; // Compression
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  std::ostringstream out;
  images::common::bitmap_header header;
  header.read(in);
  header.write(out);
  EXPECT_EQ(buffer_string, out.view());
}

TEST(bitmap_header, write_error) {
  const images::common::bitmap_header header;
  std::ostringstream out;
  out.setstate(std::ios::badbit); // Simulate write error
  EXPECT_THROW(header.write(out), images::common::file_error);
}

TEST(bitmap_header, print_info) {
  std::array<char, 100> buffer{};
  buffer[0] = 'B';
  buffer[1] = 'M';
  buffer[10] = 54; // Pixels start
  buffer[18] = 50; // Width
  buffer[22] = 100; // Height
  buffer[26] = 1; // Planes
  buffer[28] = 24; // Bit count
  buffer[30] = 0; // Compression
  const std::string buffer_string{buffer.data(), std::ssize(buffer)};
  std::stringstream in{buffer_string};
  images::common::bitmap_header header;
  header.read(in);
  std::ostringstream out;
  header.print_info(out);
  /*
  std::string expected = "    Width: 50\n    Height: 100\n    Pixel start: 54\n"
                         "    Number of planes: 1\n    Bits per pixel: 24\n    Compression: 0\n"
                         "    Extra header size: 46\n";*/
  //EXPECT_EQ(expected, out.view());
}
