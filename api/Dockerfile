FROM golang:1.25 AS build
WORKDIR /app
COPY api/ .
RUN go build -o api .

FROM ubuntu:24.04
COPY --from=build /app/api /usr/local/bin/api
CMD ["api"]
