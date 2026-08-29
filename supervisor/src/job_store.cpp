#include "job_store.h"

JobStore::JobStore(const std::string& connection_string) : connection_(connection_string)
{
}

void JobStore::ReportResult(const std::string& job_id, int exit_code)
{
  pqxx::work txn(connection_);
  txn.exec("UPDATE jobs SET status = $1, exit_code = $2 WHERE id = $3",
           pqxx::params{"completed", exit_code, job_id});
  txn.commit();
}
