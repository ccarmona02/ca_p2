add_library(aos bitmap_aos.cpp)
target_link_libraries(aos PUBLIC OpenMP::OpenMP_CXX common)
target_include_directories(aos PUBLIC ..)
