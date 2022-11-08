#include "soa/bitmap_soa.hpp"
#include "common/imgcmd.hpp"
#include "common/progargs.hpp"
#include <iostream>

int main(int argc, char **argv) {
  using namespace images::common;
  using namespace images::soa;

  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  const std::vector<std::string> args(argv, argv + argc);
  const auto config = parse_arguments(args);
  process<bitmap_soa>(config);
}

