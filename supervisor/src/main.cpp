#include <cstdlib>
#include <csignal>
#include <fstream>
#include <string>

#include "supervisor.h"

namespace
{
void LoadEnvFile(const std::string& path)
{
  std::ifstream file(path);
  std::string line;

  while (std::getline(file, line))
  {
    size_t eq = line.find('=');
    if (eq == std::string::npos) continue;

    setenv(line.substr(0, eq).c_str(), line.substr(eq + 1).c_str(), 0);
  }
}

std::string GetEnv(const std::string& key, const std::string& default_value)
{
  const char* value = std::getenv(key.c_str());
  return value ? value : default_value;
}

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
  LoadEnvFile(".env");

  Supervisor supervisor(GetEnv("SUPERVISOR_ADDRESS", "0.0.0.0:50051"), GetEnv("ETCD_ENDPOINT", "localhost:2379"),
                        GetEnv("POSTGRES_DSN", "postgres://localhost:5432/jobscheduler"));
  g_supervisor = &supervisor;

  std::signal(SIGINT, HandleSignal);
  std::signal(SIGTERM, HandleSignal);

  supervisor.Run();

  return 0;
}
