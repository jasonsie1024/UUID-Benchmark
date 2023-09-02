cmake_minimum_required(VERSION 3.0)
project("UUID Benchmark")

set(BOOST_ROOT /opt/homebrew/Cellar/boost/1.82.0_1)
# set(BOOST_INCLUDEDIR /opt/homebrew/Cellar/boost/1.82.0_1/include)
# set(BOOST_LIBRARYDIR /opt/homebrew/Cellar/boost/1.82.0_1/lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost REQUIRED COMPONENTS system)
find_package(benchmark REQUIRED)
if(Boost_FOUND AND benchmark_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
    MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(uuid_benchmark uuid_benchmark.cpp)
    target_link_libraries(uuid_benchmark ${Boost_LIBRARIES} benchmark::benchmark)
endif()
