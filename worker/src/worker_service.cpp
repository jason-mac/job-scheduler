#include "worker_service.h"

grpc::Status WorkerServiceImpl::ExecuteJob(grpc::ServerContext* context,
                                            const scheduler::ExecuteJobRequest* request,
                                            scheduler::ExecuteJobResponse* response)
{
  // TODO: implement
  return grpc::Status::OK;
}
