#!/bin/bash
# Setup script for Embedded HAL Temperature Controller

set -e

echo "=== Embedded HAL Temperature Controller Setup ==="
echo ""

# Create build directories
echo "�� Creating build directories..."
mkdir -p tests/build
mkdir -p simulation/build
echo "   ✅ tests/build created"
echo "   ✅ simulation/build created"
echo ""

# Configure tests
echo "🔧 Configuring tests..."
cd tests/build
cmake .. >/dev/null 2>&1 && echo "   ✅ Tests configured successfully" || echo "   ❌ Tests configuration failed"
cd ../..

# Configure simulation  
echo "🔧 Configuring simulation..."
cd simulation/build
cmake .. >/dev/null 2>&1 && echo "   ✅ Simulation configured successfully" || echo "   ❌ Simulation configuration failed"
cd ../..

echo ""
echo "🎯 Setup complete! Next steps:"
echo ""
echo "Run tests:"
echo "  cd tests/build && make && ./hal_tests"
echo ""
echo "Build simulation:"  
echo "  cd simulation/build && make && ./hal_simulation"
echo ""
echo "VS Code users can use:"
echo "  Ctrl+Shift+P → Tasks: Run Task → [Build Tests|Run Tests|Build Simulation]"
echo ""
echo "📖 View documentation:"
echo "  open docs/design.html"
echo ""
