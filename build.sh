#!/usr/bin/env bash
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_DIR="$SCRIPT_DIR/build_logs"
mkdir -p "$LOG_DIR"

echo "==> Building C++ (CMake) and Rust gateway (Cargo) in parallel"
echo "==> Logs: $LOG_DIR/cpp.log, $LOG_DIR/rust.log"

(
  cd "$SCRIPT_DIR"
  cmake --preset default && cmake --build --preset default
) > "$LOG_DIR/cpp.log" 2>&1 &
cpp_pid=$!

(
  cd "$SCRIPT_DIR/api"
  cargo build
) > "$LOG_DIR/rust.log" 2>&1 &
rust_pid=$!

cpp_status=0
rust_status=0

wait "$cpp_pid" || cpp_status=$?
wait "$rust_pid" || rust_status=$?

if [ "$cpp_status" -ne 0 ] || [ "$rust_status" -ne 0 ]; then
  echo "==> Build failed (cpp exit=$cpp_status, rust exit=$rust_status)"
  echo "==> See $LOG_DIR/cpp.log and $LOG_DIR/rust.log for details"
  exit 1
fi

echo "==> Done"
