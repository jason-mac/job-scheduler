#include "thread_pool.h"

#include <chrono>
#include <thread>

int main()
{
  ThreadPool pool;

  for (;;)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
