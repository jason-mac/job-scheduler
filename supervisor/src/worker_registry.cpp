#include "worker_registry.h"

WorkerRegistry::WorkerRegistry(const std::string& etcd_endpoint) : client_(etcd_endpoint) {}

WorkerRegistry::~WorkerRegistry()
{
  if (watcher_)
  {
    watcher_->Cancel();
  }
}

void WorkerRegistry::Start()
{
  etcd::Response response = client_.ls(WORKERS_PREFIX).get();

  {
    std::lock_guard<std::mutex> lock(mutex_);

    for (size_t i = 0; i < response.keys().size(); ++i)
    {
      workers_[response.keys()[i]] = response.value(static_cast<int>(i)).as_string();
    }
  }

  watcher_ = std::make_unique<etcd::Watcher>(
      client_, WORKERS_PREFIX, [this](etcd::Response response) { HandleWatchResponse(response); },
      true);
}

void WorkerRegistry::HandleWatchResponse(etcd::Response response)
{
  std::lock_guard<std::mutex> lock(mutex_);

  for (const auto& event : response.events())
  {
    if (event.event_type() == etcd::Event::EventType::PUT)
    {
      workers_[event.kv().key()] = event.kv().as_string();
    }
    else if (event.event_type() == etcd::Event::EventType::DELETE_)
    {
      workers_.erase(event.kv().key());
    }
  }
}

std::string WorkerRegistry::GetWorker()
{
  std::lock_guard<std::mutex> lock(mutex_);

  if (workers_.empty())
  {
    return "";
  }

  auto it = workers_.begin();
  std::advance(it, next_index_ % workers_.size());
  next_index_++;

  return it->second;
}
