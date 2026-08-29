#include "job_poller.h"

#include <chrono>

JobPoller::JobPoller() : poll_thread_([this](std::stop_token stop_token) { PollLoop(stop_token); })
{
}

JobPoller::~JobPoller() = default;

void JobPoller::Submit(std::future<int> job_future)
{
  std::lock_guard<std::mutex> lock(mutex_);
  pending_.push(std::move(job_future));
}

void JobPoller::PollLoop(std::stop_token stop_token)
{
  while (!stop_token.stop_requested())
  {
    {
      std::lock_guard<std::mutex> lock(mutex_);

      size_t remaining = pending_.size();
      for (size_t i = 0; i < remaining; ++i)
      {
        std::future<int> job_future = std::move(pending_.front());
        pending_.pop();

        if (job_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
          HandleResult(job_future.get());
        }
        else
        {
          pending_.push(std::move(job_future));
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void JobPoller::HandleResult(int exit_code) {}
