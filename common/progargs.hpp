#ifndef IMAGES_COMMON_PROGARGS_HPP
#define IMAGES_COMMON_PROGARGS_HPP

#include <optional>
#include <string_view>
#include <vector>
#include <filesystem>

namespace images::common {

  enum class subcommand {
    copy,
    histo,
    mono,
    gauss,
    info
  };

  std::optional<subcommand> to_subcommand(std::string_view str_cmd) noexcept;

  struct configuration {
    std::filesystem::path input_dir;
    std::filesystem::path output_dir;
    subcommand subcmd;
  };

  configuration parse_arguments(const std::vector<std::string> & args) noexcept;

}// namespace images::common

#endif//IMAGES_COMMON_PROGARGS_HPP
