#pragma once

#include "scheduler.grpc.pb.h"

class WorkerServiceImpl final : public scheduler::Worker::Service
{
public:
  grpc::Status ExecuteJob(grpc::ServerContext* context, const scheduler::ExecuteJobRequest* request,
                          scheduler::ExecuteJobResponse* response) override;
};
