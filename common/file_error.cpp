#include "file_error.hpp"

namespace images::common {

  std::string to_string(file_error_kind error) {
    switch (error) {
      case file_error_kind::cannot_open:
        return "Cannot open file";
      case file_error_kind::cannot_read:
        return "Cannot read header";
      case file_error_kind::cannot_read_extra:
        return "Cannot read extra header";
      case file_error_kind::cannot_write:
        return "Cannot write header";
      case file_error_kind::invalid_magic_number:
        return "Invalid bitmap header";
      case file_error_kind::invalid_planes:
        return "Invalid number of planes";
      case file_error_kind::invlaid_bit_count:
        return "Invalid number of bits per pixel";
      case file_error_kind::invalid_compression:
        return "Unexpected comperession level";
      case file_error_kind::invalid_pixel_start:
        return "Invalid pixel start";
      default:
        return "Unknown error";
    }
  }

}