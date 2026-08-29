#pragma once

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "job_store.h"
#include "scheduler_service.h"
#include "worker_registry.h"

class Supervisor
{
public:
  Supervisor(const std::string& address, const std::string& etcd_endpoint, const std::string& postgres_dsn);
  void Run();
  void Shutdown();
  grpc::Status HandleSubmitJob(const scheduler::SubmitJobRequest* request,
                                scheduler::SubmitJobResponse* response);
  grpc::Status HandleReportJobResult(const scheduler::ReportJobResultRequest* request,
                                      scheduler::ReportJobResultResponse* response);

private:
  std::string address_;
  WorkerRegistry worker_registry_;
  JobStore job_store_;
  SchedulerServiceImpl service_;
  std::unique_ptr<grpc::Server> server_;
};
