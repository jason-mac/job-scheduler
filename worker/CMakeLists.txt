add_library(worker_lib STATIC
  src/thread_pool.cpp
)

target_include_directories(worker_lib PUBLIC include)

add_executable(worker
  src/main.cpp
)

target_link_libraries(worker PRIVATE worker_lib)

add_subdirectory(tests)
