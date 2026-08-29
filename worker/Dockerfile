FROM ubuntu:24.04 AS build
WORKDIR /app
COPY worker/requirements.yml worker/install_deps.sh worker/
RUN worker/install_deps.sh

COPY scripts/install_etcd_cpp.sh .
RUN ./install_etcd_cpp.sh

COPY . .
RUN cmake --preset default && cmake --build --preset default

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y libprotobuf32t64 libgrpc++1 libyaml-cpp0.8 libcpprest2.10 libboost-system1.83.0 libboost-thread1.83.0
COPY --from=build /usr/local/lib/libetcd-cpp-api.so /usr/local/lib/
RUN ldconfig
COPY --from=build /app/build/worker/worker /usr/local/bin/worker
CMD ["worker"]
