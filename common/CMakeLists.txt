add_library(common progargs.cpp bitmap_header.cpp pixel.cpp histogram.cpp file_error.cpp normalized_pixel.cpp normalized_pixel.hpp)
target_include_directories(common PUBLIC ..)