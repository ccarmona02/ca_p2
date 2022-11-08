#include "progargs.hpp"
#include <iostream>
#include <filesystem>
#include <map>
#include <optional>

namespace {
  using namespace images::common;

  using namespace std::literals;

  const std::map<std::string_view, subcommand> subcommand_map{ // NOLINT(cert-err58-cpp)
      {"copy"sv,  subcommand::copy},
      {"histo"sv, subcommand::histo},
      {"mono"sv,  subcommand::mono},
      {"gauss"sv, subcommand::gauss},
      {"info"sv,  subcommand::info},
  };

  void print_format_help(std::ostream & os, std::string_view prog_name) noexcept {
    const std::filesystem::path prog{prog_name};
    os << "  " << prog.filename().native() << " in_path out_path oper\n";
    os << "    operation: copy, histo, mono, gauss, info\n";
  }

  void error_format(std::ostream & os, std::string_view prog_name) noexcept {
    os << "Wrong format:\n";
    print_format_help(os, prog_name);
    std::exit(-1);
  }

  void error_invalid_argument(std::ostream & os, std::string_view prog_name, std::string_view op)
  noexcept {
    os << "Unexpected operation:" << op << "\n";
    print_format_help(os, prog_name);
    std::exit(-1);
  }

  void error_input_missing(std::ostream & os, std::string_view prog_name, std::string_view in,
      std::string_view out) noexcept {
    os << "Input path: " << in << "\n";
    os << "Outut path: " << out << "\n";
    os << "Cannot open directory [" << in << "]\n";
    print_format_help(os, prog_name);
    std::exit(-1);
  }

  void error_output_missing(std::ostream & os, std::string_view prog_name, std::string_view in,
      std::string_view out) noexcept {
    os << "Input path: " << in << "\n";
    os << "Outut path: " << out << "\n";
    os << "Output directory [" << out << "] does not exist\n";
    print_format_help(os, prog_name);
    std::exit(-1);
  }

}

namespace images::common {

  std::optional<subcommand> to_subcommand(std::string_view str_cmd) noexcept {
    auto it = subcommand_map.find(str_cmd);
    if (it == subcommand_map.end()) { return std::nullopt; }
    auto [_, value] = *it;
    return value;
  }

  configuration parse_arguments(const std::vector<std::string> & args) noexcept {
    namespace fs = std::filesystem;

    if (std::ssize(args) != 4) {
      error_format(std::cerr, args[0]);
    }

    auto in_path = fs::path(args[1]);
    auto out_path = fs::path(args[2]);
    if (!fs::exists(in_path)) {
      error_input_missing(std::cerr, args[0], args[1], args[2]);
    }
    if (!fs::exists(out_path)) {
      error_output_missing(std::cerr, args[0], args[1], args[2]);
    }
    auto op = to_subcommand(args[3]);
    subcommand subcmd = subcommand::info;
    if (!op) { error_invalid_argument(std::cerr, args[0], args[3]); }
    else { subcmd = *op; }
    return {in_path, out_path, subcmd};
  }

}// namespace images::common