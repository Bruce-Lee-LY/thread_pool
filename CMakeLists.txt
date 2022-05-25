# Copyright 2022. All Rights Reserved.
# Author: Bruce-Lee-LY
# Date: 22:30:20 on Tue, May 17, 2022
#
# Description: cmake for thread pool

cmake_minimum_required (VERSION 3.8)

project (thread_pool LANGUAGES CXX)

set (CMAKE_VERBOSE_MAKEFILE ${VERBOSE_MAKEFILE})

set (CMAKE_CXX_FLAGS "-std=c++11")
set (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g2 -ggdb")
set (CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")

set (CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed")

add_compile_options (
    -Wall
    -Werror
    -Wextra
    -Wswitch-default
    -Wfloat-equal
    -Wshadow
    -Wcast-qual
)

include_directories (${PROJECT_SOURCE_DIR}/src)

add_executable (tp_sample ${PROJECT_SOURCE_DIR}/sample/tp_sample.cpp)
target_link_libraries (tp_sample -lpthread)

install (TARGETS tp_sample RUNTIME DESTINATION sample)
