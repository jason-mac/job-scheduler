#include "worker_service.h"

#include <sys/wait.h>
#include <unistd.h>

WorkerServiceImpl::WorkerServiceImpl(ThreadPool& thread_pool) : thread_pool_(thread_pool) {}

grpc::Status WorkerServiceImpl::ExecuteJob(grpc::ServerContext* context,
                                           const scheduler::ExecuteJobRequest* request,
                                           scheduler::ExecuteJobResponse* response)
{
  std::string command = request->command();

  thread_pool_.submit(
      [command]()
      {
        pid_t pid = fork();
        const bool is_child = pid == 0;
        const bool is_parent = pid > 0;

        if (is_child)
        {
          execlp("/bin/sh", "sh", "-c", command.c_str(), nullptr);
          _exit(127); // only reached if execlp fails
        }
        else if (is_parent)
        {
          int status;
          waitpid(pid, &status, 0);
        }
      });

  return grpc::Status::OK;
}
