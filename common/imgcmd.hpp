#ifndef IMAGES_COMMON_IMGCMD_HPP
#define IMAGES_COMMON_IMGCMD_HPP

#include "progargs.hpp"
#include "file_error.hpp"
#include <chrono>
#include <iostream>
#include <fstream>

namespace images::common {

  template<typename image_type>
  void process_image(image_type & image, images::common::subcommand subcmd) noexcept {
    switch (subcmd) {
      case images::common::subcommand::copy:
        break;
      case images::common::subcommand::mono:
        image.to_gray();
        break;
      case images::common::subcommand::gauss:
        image.gauss();
        break;
      case images::common::subcommand::info:
        [[fallthrough]];
      default:
        break;
    }
  }

  template<typename image_type>
  auto generate_output(const std::filesystem::path & out_dir, const std::filesystem::path & in_file,
      images::common::subcommand subcmd) {
    using clk = std::chrono::high_resolution_clock;
    image_type image;
    image.read(in_file);
    auto read_time = clk::now();

    if (subcmd == images::common::subcommand::info) {
      auto process_time = clk::now();
      image.print_info(std::cout);
      auto write_time = clk::now();
      return std::tuple{read_time, process_time, write_time};
    }
    if (subcmd != images::common::subcommand::histo) {
      process_image(image, subcmd);
      auto process_time = clk::now();
      image.write(out_dir / in_file.filename());
      auto write_time = clk::now();
      return std::tuple{read_time, process_time, write_time};
    }
    auto histo = image.generate_histogram();
    auto process_time = clk::now();
    std::ofstream histogram_out{out_dir / in_file.filename().replace_extension(".hst")};
    histo.write(histogram_out);
    auto write_time = clk::now();
    return std::tuple{read_time, process_time, write_time};
  }

  void print_times(auto times) noexcept {
    using namespace std::chrono;
    std::cout << " time(" << duration_cast<microseconds>(times[0]).count() << ")\n";
    std::cout << "  Load time: " << duration_cast<microseconds>(times[1]).count() << "\n";
    std::cout << "  Process time: " << duration_cast<microseconds>(times[2]).count() << '\n';
    std::cout << "  Store time: " << duration_cast<microseconds>(times[3]).count() << '\n';
  }

  template<typename image_type>
  void process_file(const std::filesystem::path & in_file, const std::filesystem::path & out_dir,
      images::common::subcommand subcmd) noexcept {
    try {
      std::cout << "File: " << in_file.string() << '\n';
      using clk = std::chrono::high_resolution_clock;
      const auto start_time = clk::now();
      const auto [read_time, process_time, write_time] = generate_output<image_type>(out_dir,
          in_file, subcmd);
      const std::array times = {write_time - start_time, read_time - start_time,
                                process_time - read_time, write_time - read_time};
      std::cout << "File: " << in_file.string();
      print_times(times);

    } catch (images::common::file_error e) {
      std::cerr << "File: " << in_file << std::endl;
      std::cerr << "  Cannot process file: " << in_file.string() << '\n';
      std::cerr << "  Reason: " << to_string(e.kind) << '\n';
    } catch (...) {
      std::cerr << "File: " << in_file << std::endl;
      std::cerr << "  Unexpected error in file " << in_file.string() << '\n';
    }
  }

  template<typename image_type>
  void process(const images::common::configuration & cfg) noexcept {
    namespace fs = std::filesystem;
    std::cout << "Input path: " << cfg.input_dir << '\n';
    std::cout << "Output path: " << cfg.output_dir << '\n';
    for (const auto & in_file: fs::directory_iterator(cfg.input_dir)) {
      process_file<image_type>(in_file, cfg.output_dir, cfg.subcmd);
    }
  }

}

#endif // IMAGES_COMMON_IMGCMD_HPP
