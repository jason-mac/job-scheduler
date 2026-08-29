#pragma once

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "scheduler_service.h"
#include "worker_registry.h"

class Supervisor
{
public:
  explicit Supervisor(const std::string& address);
  void Run();
  void Shutdown();
  grpc::Status HandleSubmitJob(const scheduler::SubmitJobRequest* request,
                                scheduler::SubmitJobResponse* response);

private:
  std::string address_;
  WorkerRegistry worker_registry_;
  SchedulerServiceImpl service_;
  std::unique_ptr<grpc::Server> server_;
};
