#pragma once

#include "job_poller.h"
#include "scheduler.grpc.pb.h"
#include "thread_pool.h"

class WorkerServiceImpl final : public scheduler::Worker::Service
{
public:
  WorkerServiceImpl(ThreadPool& thread_pool, JobPoller& job_poller);

  grpc::Status ExecuteJob(grpc::ServerContext* context, const scheduler::ExecuteJobRequest* request,
                          scheduler::ExecuteJobResponse* response) override;

private:
  ThreadPool& thread_pool_;
  JobPoller& job_poller_;
};
