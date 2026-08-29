#pragma once

#include "scheduler.grpc.pb.h"

class Scheduler;

class SchedulerServiceImpl final : public scheduler::Scheduler::Service
{
public:
  explicit SchedulerServiceImpl(::Scheduler& scheduler);

  grpc::Status SubmitJob(grpc::ServerContext* context, const scheduler::SubmitJobRequest* request,
                         scheduler::SubmitJobResponse* response) override;

private:
  ::Scheduler& scheduler_;
};
