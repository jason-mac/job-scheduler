#pragma once

#include <future>
#include <mutex>
#include <queue>
#include <thread>

class JobPoller
{
public:
  JobPoller();
  ~JobPoller();

  void Submit(std::future<int> job_future);

private:
  void PollLoop(std::stop_token stop_token);
  void HandleResult(int exit_code);

  std::mutex mutex_;
  std::queue<std::future<int>> pending_;
  std::jthread poll_thread_;
};
