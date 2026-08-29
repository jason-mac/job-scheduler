#pragma once

#include <memory>
#include <string>

#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>

class EtcdRegistrar
{
public:
  EtcdRegistrar(const std::string& etcd_endpoint, const std::string& worker_id, const std::string& worker_address);

  void Register();

private:
  etcd::Client client_;
  std::string worker_id_;
  std::string worker_address_;
  std::shared_ptr<etcd::KeepAlive> keep_alive_;

  static constexpr int LEASE_TTL_SECONDS = 10;
};
