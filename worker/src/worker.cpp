#include "worker.hpp"

Worker::Worker(const std::string& address, const std::string& etcd_endpoint, const std::string& worker_id)
    : address_(address), service_(thread_pool_, job_poller_), etcd_registrar_(etcd_endpoint, worker_id, address)
{
}

void Worker::Run()
{
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
  builder.RegisterService(&service_);
  server_ = builder.BuildAndStart();

  etcd_registrar_.Register();

  server_->Wait();
}

void Worker::Shutdown()
{
  if (server_) server_->Shutdown();
}
