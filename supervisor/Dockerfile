FROM ubuntu:24.04 AS build
WORKDIR /app
COPY supervisor/requirements.yml supervisor/install_deps.sh supervisor/
RUN supervisor/install_deps.sh

COPY scripts/install_etcd_cpp.sh .
RUN ./install_etcd_cpp.sh

COPY . .
RUN cmake --preset default && cmake --build --preset default

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y libprotobuf32t64 libgrpc++1 libcpprest2.10 libboost-system1.83.0 libboost-thread1.83.0
COPY --from=build /usr/local/lib/libetcd-cpp-api.so /usr/local/lib/
RUN ldconfig
COPY --from=build /app/build/supervisor/supervisor /usr/local/bin/supervisor
CMD ["supervisor"]
