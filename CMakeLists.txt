cmake_minimum_required(VERSION 3.13)

project(cpu_usage VERSION 1.0)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(
    include
    src
    )

set(SRCS
    src/CSVWrite.cpp
    src/UsageManager.cpp
    )

set(HEADERS
    include/CSVWrite.hpp
    include/UsageManager.hpp
    )


add_compile_options(
    -Wall
    -Wextra
    -std=c++17
    -stdlib=libc++
    )

add_library(usage STATIC ${SRCS} ${HEADERS})

add_executable(main main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(main usage Threads::Threads)

#target_link_libraries(main PRIVATE Threads::Threads)

#install(TARGETS main
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
