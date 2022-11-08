#include <gtest/gtest.h>
#include "aos/bitmap_aos.hpp"
#include "soa/bitmap_soa.hpp"
#include "common/file_error.hpp"

template<typename T>
class bitmap_test : public testing::Test {
};

using bitmap_types = ::testing::Types<images::aos::bitmap_aos, images::soa::bitmap_soa>;

TYPED_TEST_SUITE(bitmap_test, bitmap_types);

TYPED_TEST(bitmap_test, construct) {
  using namespace images::aos;
  TypeParam bm;
  EXPECT_EQ(0, bm.width());
  EXPECT_EQ(0, bm.height());
}

TYPED_TEST(bitmap_test, read) {
  namespace fs = std::filesystem;
  fs::path dir = fs::current_path() / "../../in";
  fs::path file = dir / "sabatini.bmp";
  EXPECT_TRUE(fs::exists(file));
  TypeParam bm;
  bm.read(file);
  EXPECT_EQ(1980, bm.width());
  EXPECT_EQ(1320, bm.height());
  std::ostringstream out;
  bm.print_info(out);
  EXPECT_EQ("    Width: 1980\n    Height: 1320\n    Pixel start: 138\n"
            "    Number of planes: 1\n    Bits per pixel: 24\n    Compression: 0\n"
            "    Extra header size: 84\n", out.str());
}

TYPED_TEST(bitmap_test, read_inexistent) {
  namespace fs = std::filesystem;
  fs::path dir = fs::current_path() / "../../in";
  fs::path file = dir / "none.bmp";
  EXPECT_FALSE(fs::exists(file));
  TypeParam bm;
  EXPECT_THROW(bm.read(file), images::common::file_error);
  EXPECT_EQ(0, bm.width());
  EXPECT_EQ(0, bm.height());
}

TYPED_TEST(bitmap_test, write) {
  namespace fs = std::filesystem;
  fs::path indir = fs::current_path() / "../../in";
  fs::path outdir = fs::current_path() / "../../out";
  fs::path infile = indir / "sabatini.bmp";
  fs::path outfile = outdir / "sabatini.bmp";
  EXPECT_TRUE(fs::exists(infile));
  fs::create_directory(outdir);
  TypeParam bm;
  bm.read(infile);
  bm.write(outfile);
  TypeParam bm_out;
  bm_out.read(outfile);
  EXPECT_EQ(bm, bm_out);
}

TYPED_TEST(bitmap_test, write_invalid) {
  namespace fs = std::filesystem;
  fs::path indir = fs::current_path() / "../../in";
  fs::path outdir = fs::current_path() / "../../noout";
  fs::path infile = indir / "sabatini.bmp";
  fs::path outfile = outdir / "sabatini.bmp";
  EXPECT_TRUE(fs::exists(infile));
  EXPECT_FALSE(fs::exists(outdir));
  TypeParam bm;
  bm.read(infile);
  EXPECT_THROW(bm.write(outfile), images::common::file_error);
}

TYPED_TEST(bitmap_test, to_gray) {
  namespace fs = std::filesystem;
  fs::path indir = fs::current_path() / "../../in";
  fs::path infile = indir / "sabatini.bmp";
  EXPECT_TRUE(fs::exists(infile));
  TypeParam bm;
  bm.read(infile);
  bm.to_gray();
  EXPECT_TRUE(bm.is_gray());
}

TYPED_TEST(bitmap_test, histogram) {
  TypeParam bm{4, 4};
  for (int i = 0; i < 4; ++i) {
    bm.set_pixel(i, i, {128, 0, 0});
  }
  bm.set_pixel(0, 0, {128, 127, 0});
  bm.set_pixel(0, 3, {128, 127, 0});
  auto h = bm.generate_histogram();
  EXPECT_EQ(5, h.get_red_frequency(128));
  EXPECT_EQ(11, h.get_red_frequency(0));
  EXPECT_EQ(2, h.get_green_frequency(127));
  EXPECT_EQ(14, h.get_green_frequency(0));
  EXPECT_EQ(16, h.get_blue_frequency(0));
}

TYPED_TEST(bitmap_test, gauss) {
  TypeParam bm{10, 10};
  bm.set_pixel(0, 0, {128, 128, 128});
  bm.set_pixel(5, 4, {100, 0, 0});
  bm.set_pixel(5, 5, {200, 0, 0});
  bm.gauss();
  using images::common::pixel;
  EXPECT_EQ(pixel(19, 19, 19), bm.get_pixel(0, 0));
  EXPECT_EQ(pixel(12, 12, 12), bm.get_pixel(0, 1));
  EXPECT_EQ(pixel(3, 3, 3), bm.get_pixel(0, 2));
  EXPECT_EQ(pixel(39, 0, 0), bm.get_pixel(5, 5));
}