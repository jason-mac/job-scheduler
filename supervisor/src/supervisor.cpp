#include "supervisor.h"

Supervisor::Supervisor(const std::string& address, const std::string& etcd_endpoint,
                        const std::string& postgres_dsn)
    : address_(address), worker_registry_(etcd_endpoint), job_store_(postgres_dsn), service_(*this)
{
}

grpc::Status Supervisor::HandleSubmitJob(const scheduler::SubmitJobRequest* request,
                                          scheduler::SubmitJobResponse* response)
{
  std::string worker_address = worker_registry_.GetWorker();

  if (worker_address.empty())
  {
    return grpc::Status(grpc::StatusCode::UNAVAILABLE, "no workers available");
  }

  auto channel = grpc::CreateChannel(worker_address, grpc::InsecureChannelCredentials());
  auto stub = scheduler::Worker::NewStub(channel);

  scheduler::ExecuteJobRequest execute_request;
  execute_request.set_command(request->command());

  scheduler::ExecuteJobResponse execute_response;
  grpc::ClientContext context;

  grpc::Status status = stub->ExecuteJob(&context, execute_request, &execute_response);
  if (!status.ok())
  {
    return status;
  }

  response->set_response(execute_response.response());

  return grpc::Status::OK;
}

grpc::Status Supervisor::HandleReportJobResult(const scheduler::ReportJobResultRequest* request,
                                                scheduler::ReportJobResultResponse* response)
{
  job_store_.ReportResult(request->job_id(), request->exit_code());

  return grpc::Status::OK;
}

void Supervisor::Run()
{
  worker_registry_.Start();

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
