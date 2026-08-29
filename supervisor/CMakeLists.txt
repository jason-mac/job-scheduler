find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(etcd-cpp-api CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

set(PROTO_GEN_DIR ${CMAKE_SOURCE_DIR}/build/generated)

add_library(supervisor_lib STATIC
  src/worker_registry.cpp
  src/scheduler_service.cpp
  src/supervisor.cpp
  src/job_store.cpp
  ${PROTO_GEN_DIR}/scheduler.pb.cc
  ${PROTO_GEN_DIR}/scheduler.grpc.pb.cc
)

target_include_directories(supervisor_lib PUBLIC include ${PROTO_GEN_DIR})

target_link_libraries(supervisor_lib PUBLIC
  gRPC::grpc++
  protobuf::libprotobuf
  etcd-cpp-api
  libpqxx::pqxx
)

add_executable(supervisor
  src/main.cpp
)

target_link_libraries(supervisor PRIVATE supervisor_lib)
