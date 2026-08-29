#include <csignal>

#include "scheduler.h"

namespace
{
Scheduler* g_scheduler = nullptr;

void HandleSignal(int)
{
  if (g_scheduler)
  {
    g_scheduler->Shutdown();
  }
}
} // namespace

int main()
{
  Scheduler scheduler("0.0.0.0:50051");
  g_scheduler = &scheduler;

  std::signal(SIGINT, HandleSignal);
  std::signal(SIGTERM, HandleSignal);

  scheduler.Run();

  return 0;
}
