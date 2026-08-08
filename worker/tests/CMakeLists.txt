find_package(GTest CONFIG REQUIRED)

add_executable(worker_tests
  test_thread_pool.cpp
)

target_link_libraries(worker_tests PRIVATE worker_lib GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(worker_tests)
