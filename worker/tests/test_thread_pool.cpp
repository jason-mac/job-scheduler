#include "thread_pool.h"

#include <atomic>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

namespace
{
bool waitUntil(const std::function<bool()> &condition,
               std::chrono::milliseconds timeout = std::chrono::milliseconds(2000))
{
  const auto deadline = std::chrono::steady_clock::now() + timeout;
  while (!condition())
  {
    if (std::chrono::steady_clock::now() > deadline)
    {
      return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  return true;
}
} // namespace

TEST(ThreadPoolTest, ConstructsWithNonZeroThreads)
{
  ThreadPool pool;
  EXPECT_GT(pool.getNumThreads(), 0u);
}

TEST(ThreadPoolTest, RunsSubmittedTask)
{
  ThreadPool pool;
  std::atomic<bool> ran{false};

  pool.submit([&ran] { ran = true; });

  EXPECT_TRUE(waitUntil([&ran] { return ran.load(); }));
}

TEST(ThreadPoolTest, RunsManyTasksAcrossThreads)
{
  ThreadPool pool;
  constexpr int kNumTasks = 200;
  std::atomic<int> counter{0};

  for (int i = 0; i < kNumTasks; ++i)
  {
    pool.submit([&counter] { counter.fetch_add(1); });
  }

  EXPECT_TRUE(waitUntil([&counter] { return counter.load() == kNumTasks; }));
  EXPECT_EQ(counter.load(), kNumTasks);
}

TEST(ThreadPoolTest, DestructsCleanlyWithPendingWork)
{
  std::atomic<int> counter{0};

  {
    ThreadPool pool;
    for (int i = 0; i < 20; ++i)
    {
      pool.submit([&counter] { counter.fetch_add(1); });
    }
  } // destructor must request-stop, wake sleepers, and join without deadlocking

  SUCCEED();
}

TEST(ThreadPoolTest, MultipleWorkersReportedByGetNumThreads)
{
  ThreadPool pool;
  EXPECT_EQ(pool.getNumThreads(),
            std::max(std::thread::hardware_concurrency(), 4u));
}
