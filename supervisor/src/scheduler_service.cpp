#include "scheduler_service.h"

#include "supervisor.h"

SchedulerServiceImpl::SchedulerServiceImpl(::Supervisor& supervisor) : supervisor_(supervisor) {}

grpc::Status SchedulerServiceImpl::SubmitJob(grpc::ServerContext* context,
                                             const scheduler::SubmitJobRequest* request,
                                             scheduler::SubmitJobResponse* response)
{
  return supervisor_.HandleSubmitJob(request, response);
}
