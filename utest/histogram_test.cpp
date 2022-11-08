#include <gtest/gtest.h>
#include "common/histogram.hpp"

TEST(histogram, default_construct) {
  using namespace images::common;
  const histogram h;
  std::ostringstream out;
  h.write(out);
  std::istringstream in{out.str()};
  for (int i = 0; in and i < 768; ++i) {
    int value;
    in >> value;
    EXPECT_EQ(0, value);
  }
  EXPECT_TRUE(in);
}

TEST(histogram, add_color) {
  using namespace images::common;
  histogram h;
  h.add_color({10, 10, 10});
  h.add_color({10, 10, 10});
  std::ostringstream out;
  h.write(out);
  std::istringstream in{out.str()};
  for (int i = 0; in and i < 768; ++i) {
    int value;
    in >> value;
    if (i % 256 == 10) { EXPECT_EQ(2, value); }
    else { EXPECT_EQ(0, value); }
  }
  EXPECT_TRUE(in);
}
