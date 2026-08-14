#!/usr/bin/env bash
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_DIR="$SCRIPT_DIR/build_logs"
mkdir -p "$LOG_DIR"

echo "==> Building C++ (CMake) and Go API in parallel"
echo "==> Logs: $LOG_DIR/cpp.log, $LOG_DIR/go.log"

(
  cd "$SCRIPT_DIR"
  cmake --preset default && cmake --build --preset default
) > "$LOG_DIR/cpp.log" 2>&1 &
cpp_pid=$!

(
  cd "$SCRIPT_DIR/api"
  go build ./...
) > "$LOG_DIR/go.log" 2>&1 &
go_pid=$!

cpp_status=0
go_status=0

wait "$cpp_pid" || cpp_status=$?
wait "$go_pid" || go_status=$?

if [ "$cpp_status" -ne 0 ] || [ "$go_status" -ne 0 ]; then
  echo "==> Build failed (cpp exit=$cpp_status, go exit=$go_status)"
  echo "==> See $LOG_DIR/cpp.log and $LOG_DIR/go.log for details"
  exit 1
fi

echo "==> Done"
