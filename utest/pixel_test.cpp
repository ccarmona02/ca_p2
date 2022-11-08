#include <gtest/gtest.h>
#include "common/pixel.hpp"

TEST(pixel, default_construct) {
  using namespace images::common;
  pixel p{};
  EXPECT_EQ(0, p.red());
  EXPECT_EQ(0, p.green());
  EXPECT_EQ(0, p.blue());
}

TEST(pixel, construct) {
  using namespace images::common;
  pixel p{10, 20, 30};
  EXPECT_EQ(10, p.red());
  EXPECT_EQ(20, p.green());
  EXPECT_EQ(30, p.blue());
}

TEST(pixel, modifiers) {
  using namespace images::common;
  pixel p;
  p.red(10);
  p.green(20);
  p.blue(30);
  EXPECT_EQ(10, p.red());
  EXPECT_EQ(20, p.green());
  EXPECT_EQ(30, p.blue());
}

TEST(pixel, to_grey_black) {
  using namespace images::common;
  pixel p{};
  auto q = p.to_gray_corrected();
  EXPECT_EQ(0, p.red());
  EXPECT_EQ(0, p.green());
  EXPECT_EQ(0, p.blue());
  EXPECT_EQ(0, q.red());
  EXPECT_EQ(0, q.green());
  EXPECT_EQ(0, q.blue());
}


TEST(pixel, to_grey_white) {
  using namespace images::common;
  const pixel p{255, 255, 255};
  auto q = p.to_gray_corrected();
  EXPECT_EQ(255, p.red());
  EXPECT_EQ(255, p.green());
  EXPECT_EQ(255, p.blue());
  EXPECT_EQ(254, q.red());
  EXPECT_EQ(254, q.green());
  EXPECT_EQ(254, q.blue());
}

TEST(pixel, to_grey_red) {
  using namespace images::common;
  pixel p{255, 0, 0};
  auto q = p.to_gray_corrected();
  EXPECT_EQ(255, p.red());
  EXPECT_EQ(0, p.green());
  EXPECT_EQ(0, p.blue());
  EXPECT_EQ(127, q.red());
  EXPECT_EQ(127, q.green());
  EXPECT_EQ(127, q.blue());
}

TEST(pixel, to_grey_other) {
  using namespace images::common;
  pixel p{27, 32, 25};
  auto q = p.to_gray_corrected();
  EXPECT_EQ(27, p.red());
  EXPECT_EQ(32, p.green());
  EXPECT_EQ(25, p.blue());
  EXPECT_EQ(30, q.red());
  EXPECT_EQ(30, q.green());
  EXPECT_EQ(30, q.blue());
}

TEST(pixel, write) {
  using namespace images::common;
  pixel p{10, 20, 30};
  std::ostringstream out;
  p.write(out);
  std::array<char, 3> buffer{30, 20, 10};
  std::string_view buffer_view{buffer.data(), std::size(buffer)};
  EXPECT_EQ(buffer_view, out.view());
}

TEST(pixel, read) {
  using namespace images::common;
  std::array<char, 3> buffer{30, 20, 10};
  std::string buffer_view{buffer.data(), std::size(buffer)};
  pixel p{};
  std::istringstream in{buffer_view};
  p.read(in);
  EXPECT_EQ(10, p.red());
  EXPECT_EQ(20, p.green());
  EXPECT_EQ(30, p.blue());
}
