FROM ubuntu:24.04 AS build
WORKDIR /app
COPY supervisor/requirements.yml supervisor/install_deps.sh supervisor/
RUN supervisor/install_deps.sh
COPY . .
RUN cmake --preset default && cmake --build --preset default

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y libprotobuf32 libgrpc++1
COPY --from=build /app/build/supervisor/supervisor /usr/local/bin/supervisor
CMD ["supervisor"]
