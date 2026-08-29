#include "scheduler.h"

Scheduler::Scheduler(const std::string& address) : address_(address), service_(*this) {}

grpc::Status Scheduler::HandleSubmitJob(const scheduler::SubmitJobRequest* request,
                                         scheduler::SubmitJobResponse* response)
{
  // TODO: implement — use worker_registry_ to pick a worker, dispatch, fill in response
  return grpc::Status::OK;
}

void Scheduler::Run()
{
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
  builder.RegisterService(&service_);
  server_ = builder.BuildAndStart();
  server_->Wait();
}

void Scheduler::Shutdown()
{
  if (server_) server_->Shutdown();
}
