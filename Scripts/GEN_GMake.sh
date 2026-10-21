#!/usr/bin/env bash

set -e

# Switch to the repository root, regardless of the current directory.
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

# Path to the Premake executable.
PREMAKE="$REPO_ROOT/Vendor/premake/bin/premake5"

# Check whether Premake exists.
if [[ ! -x "$PREMAKE" ]]; then
    echo "[ERROR] Premake executable not found or not executable:"
    echo "        $PREMAKE"
    exit 1
fi

# Generate GNU Make 2 project files.
echo "[INFO] Generating GNU Make project files..."

"$PREMAKE" --build-dir=gmake gmake

echo
echo "[SUCCESS] Premake finished successfully."
echo "[INFO] Build files are located in: $REPO_ROOT/build/gmake2"