#include "worker_service.h"

#include <sys/wait.h>
#include <unistd.h>

#include <future>
#include <memory>

WorkerServiceImpl::WorkerServiceImpl(ThreadPool& thread_pool, JobPoller& job_poller)
    : thread_pool_(thread_pool), job_poller_(job_poller)
{
}

grpc::Status WorkerServiceImpl::ExecuteJob(grpc::ServerContext* context,
                                           const scheduler::ExecuteJobRequest* request,
                                           scheduler::ExecuteJobResponse* response)
{
  std::string command = request->command();

  auto task = std::make_shared<std::packaged_task<int()>>(
      [command]()
      {
        pid_t pid = fork();
        const bool is_child = pid == 0;
        const bool is_parent = pid > 0;

        if (is_child)
        {
          execlp("/bin/sh", "sh", "-c", command.c_str(), nullptr);
          _exit(127);
        }

        if (is_parent)
        {
          int status;
          waitpid(pid, &status, 0);
          return status;
        }

        return -1;
      });

  std::future<int> future = task->get_future();
  thread_pool_.submit([task]() { (*task)(); });
  job_poller_.Submit(std::move(future));

  return grpc::Status::OK;
}
