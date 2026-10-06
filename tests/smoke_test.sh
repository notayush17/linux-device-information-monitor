#!/bin/sh
set -eu
output="$(./build/hw_monitor --demo)"
printf '%s\n' "$output" | grep -q "Linux Device Information Monitor"
printf '%s\n' "$output" | grep -q "CPU cores"
printf '%s\n' "$output" | grep -q "Memory total"
echo "Smoke test passed"
