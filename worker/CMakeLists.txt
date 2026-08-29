find_package(yaml-cpp CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_GEN_DIR ${CMAKE_SOURCE_DIR}/build/generated)

add_library(worker_lib STATIC
  src/thread_pool.cpp
  src/worker_service.cpp
  ${PROTO_GEN_DIR}/scheduler.pb.cc
  ${PROTO_GEN_DIR}/scheduler.grpc.pb.cc
)

target_include_directories(worker_lib PUBLIC include ${PROTO_GEN_DIR})

target_link_libraries(worker_lib PUBLIC
  yaml-cpp::yaml-cpp
  gRPC::grpc++
  protobuf::libprotobuf
)

add_executable(worker
  src/main.cpp
)

target_link_libraries(worker PRIVATE worker_lib)

add_subdirectory(tests)
