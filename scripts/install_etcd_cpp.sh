#!/usr/bin/env bash
set -euo pipefail

git clone --depth 1 https://github.com/etcd-cpp-apiv3/etcd-cpp-apiv3.git /tmp/etcd-cpp-apiv3
cmake -S /tmp/etcd-cpp-apiv3 -B /tmp/etcd-cpp-apiv3/build -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/etcd-cpp-apiv3/build -j"$(nproc)"
cmake --install /tmp/etcd-cpp-apiv3/build
rm -rf /tmp/etcd-cpp-apiv3
