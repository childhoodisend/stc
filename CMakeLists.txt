cmake_minimum_required(VERSION 3.10)
project(stc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wfloat-conversion -Werror -pedantic")
include_directories(lib lib/diff lib/rectpulse lib/psk_mod lib/count_more_than lib/find_local_peaks)
add_executable(stc
        main.cpp
        lib/diff/diff.cpp
        lib/diff/diff.h
        lib/rectpulse/rectpulse.cpp
        lib/rectpulse/rectpulse.h
        lib/psk_mod/psk_mod.cpp
        lib/psk_mod/psk_mod.h
        lib/count_more_than/count_more_than.cpp
        lib/count_more_than/count_more_than.h
        lib/find_local_peaks/find_local_peaks.cpp
        lib/find_local_peaks/find_local_peaks.h)
