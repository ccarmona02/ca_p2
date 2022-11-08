#include <gtest/gtest.h>
#include "common/file_error.hpp"

TEST(file_error_to_string, cannot_open) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::cannot_open);
  EXPECT_EQ("Cannot open file", msg);
}

TEST(file_error_to_string, cannot_read) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::cannot_read);
  EXPECT_EQ("Cannot read header", msg);
}

TEST(file_error_to_string, cannot_read_extra) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::cannot_read_extra);
  EXPECT_EQ("Cannot read extra header", msg);
}

TEST(file_error_to_string, cannot_write) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::cannot_write);
  EXPECT_EQ("Cannot write header", msg);
}

TEST(file_error_to_string, invalid_magic_number) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::invalid_magic_number);
  EXPECT_EQ("Invalid bitmap header", msg);
}

TEST(file_error_to_string, invalid_planes) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::invalid_planes);
  EXPECT_EQ("Invalid number of planes", msg);
}

TEST(file_error_to_string, invalid_bit_count) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::invlaid_bit_count);
  EXPECT_EQ("Invalid number of bits per pixel", msg);
}

TEST(file_error_to_string, invalid_compression) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::invalid_compression);
  EXPECT_EQ("Unexpected comperession level", msg);
}

TEST(file_error_to_string, invalid_pixel_start) {
  using namespace images::common;
  auto msg = to_string(file_error_kind::invalid_pixel_start);
  EXPECT_EQ("Invalid pixel start", msg);
}

TEST(file_error_to_string, unkonwn_file_error) {
  using namespace images::common;
  auto msg = to_string(file_error_kind{-1});
  EXPECT_EQ("Unknown error", msg);
}