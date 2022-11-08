include(GoogleTest)

add_executable(utest
               file_error_test.cpp bitmap_header_test.cpp
               progargs_test.cpp pixel_test.cpp
               histogram_test.cpp
               bitmap_test.cpp)
target_link_libraries(utest PRIVATE common aos soa GTest::gtest GTest::gtest_main)
target_include_directories(utest PRIVATE ..)

gtest_discover_tests(utest)
