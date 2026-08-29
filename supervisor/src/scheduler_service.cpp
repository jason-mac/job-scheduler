#include "scheduler_service.h"

#include "scheduler.h"

SchedulerServiceImpl::SchedulerServiceImpl(::Scheduler& scheduler) : scheduler_(scheduler) {}

grpc::Status SchedulerServiceImpl::SubmitJob(grpc::ServerContext* context,
                                             const scheduler::SubmitJobRequest* request,
                                             scheduler::SubmitJobResponse* response)
{
  return scheduler_.HandleSubmitJob(request, response);
}
