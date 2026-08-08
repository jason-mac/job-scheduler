add_library(supervisor_lib STATIC
  src/worker_registry.cpp
)

target_include_directories(supervisor_lib PUBLIC include)
