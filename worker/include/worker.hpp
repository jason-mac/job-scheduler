#pragma once

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "etcd_registrar.h"
#include "thread_pool.h"
#include "worker_service.h"

class Worker
{
public:
  Worker(const std::string& address, const std::string& etcd_endpoint, const std::string& worker_id);
  void Run();
  void Shutdown();

private:
  std::string address_;
  ThreadPool thread_pool_;
  WorkerServiceImpl service_;
  EtcdRegistrar etcd_registrar_;
  std::unique_ptr<grpc::Server> server_;
};
