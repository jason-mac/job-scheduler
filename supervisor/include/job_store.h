#pragma once

#include <string>

#include <pqxx/pqxx>

class JobStore
{
public:
  explicit JobStore(const std::string& connection_string);

  void ReportResult(const std::string& job_id, int exit_code);

private:
  pqxx::connection connection_;
};
