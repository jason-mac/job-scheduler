add_library(worker_lib STATIC
  src/thread_pool.cpp
)

target_include_directories(worker_lib PUBLIC include)
