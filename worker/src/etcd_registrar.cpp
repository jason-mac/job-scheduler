#include "etcd_registrar.h"

EtcdRegistrar::EtcdRegistrar(const std::string& etcd_endpoint, const std::string& worker_id,
                              const std::string& worker_address)
    : client_(etcd_endpoint), worker_id_(worker_id), worker_address_(worker_address)
{
}

void EtcdRegistrar::Register()
{
  keep_alive_ = client_.leasekeepalive(LEASE_TTL_SECONDS).get();
  int64_t lease_id = keep_alive_->Lease();

  client_.put("/workers/" + worker_id_, worker_address_, lease_id).wait();
}
