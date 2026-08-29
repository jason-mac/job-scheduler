#pragma once

#include "scheduler.grpc.pb.h"
#include "thread_pool.h"

class WorkerServiceImpl final : public scheduler::Worker::Service
{
public:
  explicit WorkerServiceImpl(ThreadPool& thread_pool);

  grpc::Status ExecuteJob(grpc::ServerContext* context, const scheduler::ExecuteJobRequest* request,
                          scheduler::ExecuteJobResponse* response) override;

private:
  ThreadPool& thread_pool_;
};
