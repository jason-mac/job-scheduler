#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PACKAGES=$(grep '^\s*- ' "$SCRIPT_DIR/requirements.yml" | sed 's/^\s*- //')

apt-get update
apt-get install -y $PACKAGES
