#!/bin/bash

# Build and run tests script
set -e

echo "=== Building Unit Tests ==="

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build tests
make -j$(nproc)

echo ""
echo "=== Running Unit Tests ==="

# Run the tests
./unit_tests

echo ""
echo "=== Test Summary ==="
echo "All tests completed successfully!"
