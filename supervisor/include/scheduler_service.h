#pragma once

#include "scheduler.grpc.pb.h"

class Supervisor;

class SchedulerServiceImpl final : public scheduler::Scheduler::Service
{
public:
  explicit SchedulerServiceImpl(::Supervisor& supervisor);

  grpc::Status SubmitJob(grpc::ServerContext* context, const scheduler::SubmitJobRequest* request,
                         scheduler::SubmitJobResponse* response) override;

private:
  ::Supervisor& supervisor_;
};
