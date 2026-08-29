#include <cstdlib>
#include <csignal>
#include <fstream>
#include <string>

#include "worker.hpp"

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
  LoadEnvFile(".env");

  Worker worker(GetEnv("WORKER_ADDRESS", "0.0.0.0:50052"), GetEnv("ETCD_ENDPOINT", "localhost:2379"),
                GetEnv("WORKER_ID", "worker-1"));
  g_worker = &worker;

  std::signal(SIGINT, HandleSignal);
  std::signal(SIGTERM, HandleSignal);

  worker.Run();

  return 0;
}
