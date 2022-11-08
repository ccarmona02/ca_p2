#ifndef IMAGES_COMMON_FILE_ERROR_HPP
#define IMAGES_COMMON_FILE_ERROR_HPP

#include <string>

namespace images::common {

  enum class file_error_kind {
    cannot_open,
    cannot_read,
    cannot_read_extra,
    cannot_write,
    invalid_magic_number,
    invalid_planes,
    invlaid_bit_count,
    invalid_compression,
    invalid_pixel_start
  };

  std::string to_string(file_error_kind error);

  struct file_error {
    file_error_kind kind;
  };

}

#endif //IMAGES_COMMON_FILE_ERROR_HPP
