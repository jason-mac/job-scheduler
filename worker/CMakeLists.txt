add_library(worker_lib STATIC
  src/thread_pool.cpp
)

target_include_directories(worker_lib PUBLIC include)

find_package(GTest CONFIG REQUIRED)

add_executable(worker_tests
  tests/thread_pool_test.cpp
)

target_link_libraries(worker_tests PRIVATE worker_lib GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(worker_tests)
