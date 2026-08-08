#!/usr/bin/env bash
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

run_worker=false
run_supervisor=false
run_api=false
run_e2e=false

if [ "$#" -eq 0 ]; then
  run_worker=true
  run_supervisor=true
  run_api=true
  run_e2e=true
fi

for arg in "$@"; do
  case "$arg" in
    --worker) run_worker=true ;;
    --supervisor) run_supervisor=true ;;
    --api) run_api=true ;;
    --e2e) run_e2e=true ;;
    *)
      echo "Unknown flag: $arg" >&2
      echo "Usage: $0 [--worker] [--supervisor] [--api] [--e2e]" >&2
      exit 1
      ;;
  esac
done

status=0

if [ "$run_worker" = true ]; then
  echo "==> worker tests"
  "$SCRIPT_DIR/build/worker/tests/worker_tests" || status=1
fi

if [ "$run_supervisor" = true ]; then
  echo "==> supervisor tests"
  echo "    (none yet)"
fi

if [ "$run_api" = true ]; then
  echo "==> api (Rust) tests"
  (cd "$SCRIPT_DIR/api" && cargo test) || status=1
fi

if [ "$run_e2e" = true ]; then
  echo "==> e2e tests"
  echo "    (none yet)"
fi

exit $status
