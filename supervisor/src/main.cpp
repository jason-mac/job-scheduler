#include <csignal>

#include "supervisor.h"

namespace
{
Supervisor* g_supervisor = nullptr;

void HandleSignal(int)
{
  if (g_supervisor)
  {
    g_supervisor->Shutdown();
  }
}
} // namespace

int main()
{
  Supervisor supervisor("0.0.0.0:50051");
  g_supervisor = &supervisor;

  std::signal(SIGINT, HandleSignal);
  std::signal(SIGTERM, HandleSignal);

  supervisor.Run();

  return 0;
}
