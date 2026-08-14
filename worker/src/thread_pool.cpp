#include "thread_pool.h"

ThreadPool::ThreadPool()
{
  unsigned int numThreads = std::max(std::thread::hardware_concurrency(), NUM_DEFAULT_THREADS);
  for (unsigned int i = 0; i < numThreads; ++i)
  {
    thread_pool_.push_back(spawnWorkerThread());
  }
}

ThreadPool::~ThreadPool()
{
  stop_source_.request_stop();
}

std::jthread ThreadPool::spawnWorkerThread()
{
  const auto worker_thread = [this]() -> void
  {
    for (;;)
    {
      std::unique_lock<std::mutex> lock(this->mutex_);
      const bool has_task = this->task_available_.wait(lock, this->stop_source_.get_token(), [this]
                                                       { return !this->task_queue_.empty(); });
      if (!has_task) return;
      auto task = std::move(this->task_queue_.front());
      this->task_queue_.pop();
      lock.unlock();
      task();
    }
  };
  return std::jthread(worker_thread);
}

unsigned int ThreadPool::getNumThreads()
{
  return thread_pool_.size();
}

void ThreadPool::submit(std::function<void()> task)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    task_queue_.push(task);
  }
  task_available_.notify_one();
}
