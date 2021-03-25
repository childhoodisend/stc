cmake_minimum_required(VERSION 3.10)
project(stc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wfloat-conversion -Werror -pedantic")
include_directories(lib)
add_executable(stc
        main.cpp
#        Tasks 1..5
        lib/tasks/diff/diff.hpp
        lib/tasks/rectpulse/rectpulse.hpp
        lib/tasks/psk_mod/psk_mod.hpp
        lib/tasks/count_more_than/count_more_than.hpp
        lib/tasks/find_local_peaks/find_local_peaks.hpp
#        File input output
        lib/file_puts.hpp
#        Types(IQF)
        lib/types.hpp
#        Tests(1..5)
        lib/tasks/tests.cpp
        lib/tasks/tests.hpp
#        Printing data
        lib/print.hpp
#        Check the tests(1..5)
        lib/tasks/checker.hpp

#        Wav files lib
        lib/wave_lib/wave.hpp lib/print.cpp)
