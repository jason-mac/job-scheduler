#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

class ThreadPool
{
public:
  ThreadPool();
  ~ThreadPool();
  unsigned int getNumThreads();
  void submit(std::function<void()> task);

private:
  std::jthread spawnWorkerThread();

  // ORDER MATTERS, DO NOT REARRANGE
  std::stop_source stop_source_;
  std::condition_variable_any task_available_;
  std::mutex mutex_;
  std::queue<std::function<void()>> task_queue_;
  std::vector<std::jthread> thread_pool_;

  static constexpr unsigned int NUM_DEFAULT_THREADS = 4;
};
