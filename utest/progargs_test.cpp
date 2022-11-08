#include <gtest/gtest.h>
#include "common/progargs.hpp"

TEST(progargs, to_subcommand_copy) {
  using namespace images::common;
  auto subcmd = to_subcommand("copy");
  EXPECT_EQ(subcommand::copy, subcmd);
}

TEST(progargs, to_subcommand_mono) {
  using namespace images::common;
  auto subcmd = to_subcommand("mono");
  EXPECT_EQ(subcommand::mono, subcmd);
}

TEST(progargs, to_subcommand_gauss) {
  using namespace images::common;
  auto subcmd = to_subcommand("gauss");
  EXPECT_EQ(subcommand::gauss, subcmd);
}

TEST(progargs, to_subcommand_histo) {
  using namespace images::common;
  auto subcmd = to_subcommand("histo");
  EXPECT_EQ(subcommand::histo, subcmd);
}

TEST(progargs, to_subcommand_info) {
  using namespace images::common;
  auto subcmd = to_subcommand("info");
  EXPECT_EQ(subcommand::info, subcmd);
}

TEST(progargs, to_subcommand_unkown) {
  using namespace images::common;
  auto subcmd = to_subcommand("error");
  EXPECT_EQ(std::nullopt, subcmd);
}

TEST(progargs, missing_arguments) {
  std::vector<std::string> args{"img", "in", "out"};
  EXPECT_DEATH({
    auto conf = images::common::parse_arguments(args);
  }, "");
}

TEST(progargs, missing_inputdir) {
  std::vector<std::string> args{"img", "unknown", "out", "copy"};
  EXPECT_DEATH({
    auto conf = images::common::parse_arguments(args);
  }, "");
}

TEST(progargs, missing_outputdir) {
  std::filesystem::create_directory("in");
  std::vector<std::string> args{"img", "in", "unknown", "copy"};
  EXPECT_DEATH({
    auto conf = images::common::parse_arguments(args);
  }, "");
}

TEST(progargs, unknown_subcmd) {
  std::filesystem::create_directory("in");
  std::filesystem::create_directory("out");
  std::vector<std::string> args{"img", "in", "out", "unknown"};
  EXPECT_DEATH({
    auto conf = images::common::parse_arguments(args);
  }, "");
}

TEST(progargs, correct_config) {
  std::filesystem::create_directory("in");
  std::filesystem::create_directory("out");
  std::vector<std::string> args{"img", "in", "out", "copy"};
  auto conf = images::common::parse_arguments(args);
  EXPECT_EQ("in", conf.input_dir);
  EXPECT_EQ("out", conf.output_dir);
  EXPECT_EQ(images::common::subcommand::copy, conf.subcmd);
}