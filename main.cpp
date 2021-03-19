//
// Created by artem on 06.03.2021.
//

#include <iostream>
//#include <vector>
//#include <string>

//#include <count_more_than/count_more_than.hpp>
//#include <diff/diff.hpp>
//#include <find_local_peaks/find_local_peaks.hpp>
//#include <psk_mod/psk_mod.hpp>
//#include <rectpulse/rectpulse.hpp>

//#include <types.hpp>
//#include <file_puts.hpp>
#include <tasks/tests.hpp>


#include <wave_lib/wave.h>


int main(){
//    test::test1();
//    test::test2();
//    test::test3();
//    test::test4();
//    test::test5();


    wave_format w;
    w.wav_reader();
    w.print_all_info();
    w.print_info();
    return EXIT_SUCCESS;
}