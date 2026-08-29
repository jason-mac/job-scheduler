#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include <etcd/Client.hpp>
#include <etcd/Watcher.hpp>

class WorkerRegistry
{
public:
  explicit WorkerRegistry(const std::string& etcd_endpoint);
  ~WorkerRegistry();

  void Start();
  std::string GetWorker();

private:
  void HandleWatchResponse(etcd::Response response);

  etcd::Client client_;
  std::unique_ptr<etcd::Watcher> watcher_;
  std::mutex mutex_;
  std::map<std::string, std::string> workers_;
  size_t next_index_ = 0;

  static constexpr const char* WORKERS_PREFIX = "/workers/";
};
