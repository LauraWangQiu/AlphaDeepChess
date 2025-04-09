#!/bin/bash
set -e
#sudo perf record -e cache-misses "build/release/AlphaDeepChess"
#sudo perf report
sudo perf record -g "build/release/AlphaDeepChess"
sudo perf report -g --no-children