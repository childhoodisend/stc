cmake_minimum_required(VERSION 3.10)
project(stc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wfloat-conversion -Werror -pedantic")
include_directories(lib file_lib)
add_executable(stc
        main.cpp
        lib/diff/diff.hpp
        lib/rectpulse/rectpulse.hpp
        lib/psk_mod/psk_mod.hpp
        lib/count_more_than/count_more_than.hpp
        lib/find_local_peaks/find_local_peaks.hpp
        file_lib/file_puts.cpp
        file_lib/file_puts.h lib/types.hpp)
