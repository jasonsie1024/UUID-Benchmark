#!/usr/bin/env bash
cmake -B "build"  -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -G Ninja &&
(cd "build" && ninja)