cmake_minimum_required(VERSION 3.0)
project("UUID Benchmark")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost REQUIRED)
find_package(benchmark REQUIRED)

add_executable(uuid_benchmark uuid_benchmark.cpp)
target_link_libraries(uuid_benchmark PRIVATE Boost::boost benchmark::benchmark benchmark::benchmark_main)