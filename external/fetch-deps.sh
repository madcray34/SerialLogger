#!/bin/sh
set -e

# Fetch dependency tarballs for environments where Zig cannot use HTTPS.
# Run this before `zig build` if `zig build --fetch` fails with TLS errors.

mkdir -p "$(dirname "$0")"
cd "$(dirname "$0")"

BOOST_URL="https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.gz"
GTEST_URL="https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz"

echo "Downloading Boost..."
curl -L -o boost_1_86_0.tar.gz "$BOOST_URL"

echo "Downloading GoogleTest..."
curl -L -o googletest-1.15.2.tar.gz "$GTEST_URL"

echo "Done."
