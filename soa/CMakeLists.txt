add_library(soa bitmap_soa.cpp)
target_link_libraries(soa PUBLIC OpenMP::OpenMP_CXX common)
target_include_directories(soa PUBLIC ..)
