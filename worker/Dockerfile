FROM ubuntu:24.04 AS build
WORKDIR /app
COPY worker/requirements.yml worker/install_deps.sh worker/
RUN worker/install_deps.sh
COPY . .
RUN cmake --preset default && cmake --build --preset default

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y libprotobuf32 libgrpc++1 libyaml-cpp0.8
COPY --from=build /app/build/worker/worker /usr/local/bin/worker
CMD ["worker"]
