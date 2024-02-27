cmake_minimum_required(VERSION 3.13)

project(cpu_usage VERSION 1.0)

include_directories(
    include
    src
    )

set(SRCS
    src/CSVWrite.cpp
    src/UsageManager.cpp
    src/Collector.cpp
    )

set(HEADERS
    include/CSVWrite.hpp
    include/UsageManager.hpp
    include/Collector.hpp
    )


add_compile_options(
    -Wall
    -Wextra
    -std=c++17
    )

add_library(usage STATIC ${SRCS} ${HEADERS})

add_executable(main main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(main usage Threads::Threads)
