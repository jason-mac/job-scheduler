#include <csignal>

#include "worker.hpp"

namespace
{
Worker* g_worker = nullptr;

void HandleSignal(int)
{
  if (g_worker)
  {
    g_worker->Shutdown();
  }
}
}  // namespace

int main()
{
  Worker worker("0.0.0.0:50052", "localhost:2379", "worker-1");
  g_worker = &worker;

  std::signal(SIGINT, HandleSignal);
  std::signal(SIGTERM, HandleSignal);

  worker.Run();

  return 0;
}
