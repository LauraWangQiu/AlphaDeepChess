#!/bin/bash
set -e
sudo perf record -g "build/release/AlphaDeepChess"
sudo perf report -g --no-children