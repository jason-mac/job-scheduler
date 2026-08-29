#include "supervisor.h"

Supervisor::Supervisor(const std::string& address) : address_(address), service_(*this) {}

grpc::Status Supervisor::HandleSubmitJob(const scheduler::SubmitJobRequest* request,
                                          scheduler::SubmitJobResponse* response)
{
  // TODO: implement — use worker_registry_ to pick a worker, dispatch, fill in response
  return grpc::Status::OK;
}

void Supervisor::Run()
{
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
  builder.RegisterService(&service_);
  server_ = builder.BuildAndStart();
  server_->Wait();
}

void Supervisor::Shutdown()
{
  if (server_) server_->Shutdown();
}
