//
// Created by artem on 13.03.2021.
//

#include <vector>
#include <algorithm>

#include <file_puts.hpp>

#include <count_more_than/count_more_than.hpp>
#include <diff/diff.hpp>
#include <find_local_peaks/find_local_peaks.hpp>
#include <psk_mod/psk_mod.hpp>
#include <rectpulse/rectpulse.hpp>

#include "tests.hpp"

#include <print.hpp>
#include <checker.hpp>

/*
 * Тесты для тасков 1-5
 */

void test::test1(){

    std::cout << "Testing Task1..." << std::endl;

    std::vector<float> v_in  = file::binput_T<float>("../source/Task1/v_in_float_Real.flt");
    std::vector<float> v_out = file::binput_T<float>("../source/Task1/v_out_float_Real.flt");


    //Implement diff method
    std::vector<float> v_diff;
    diff<float>(v_in, v_diff);


    // Проверяем, что соответствующие значения совпадают с заранее заданным checker::EPS
    if(checher::check_result<float>(v_out, v_diff)){
        std::cout << "Success! " << std::endl;
        //    printer::print_T(v_diff);
    }

}


void test::test2() {

    std::cout << "Testing Task2..." << std::endl;

    size_t nsamp = 4;
    std::vector<IQF> v_in  = file::binput_IQF("../source/Task2/v_in_float_IQ.flt");
    std::vector<IQF> v_out = file::binput_IQF("../source/Task2/v_out_float_IQ.flt");

    //Implement rectpulse method
    std::vector<IQF> v_out_rect;
    rectpulse<IQF>(v_in,v_out_rect, nsamp);


    if(checher::check_IQF(v_out, v_out_rect)){
        std::cout << "Success! " << std::endl;
        //    printer::print_IQF(v_out_rect);
    }


}

void test::test3() {

    std::cout << "Testing Task3..." << std::endl;

    size_t M = 8;

    std::vector<int> v_in  = file::binput_T<int>("../source/Task3/v_in_int_Real.flt");
    std::vector<IQF> v_out = file::binput_IQF("../source/Task3/v_out_float_IQ.flt");

    //Implement psk_mod method
    std::vector<IQF> v_out_psk;
    psk_mod(v_in, v_out_psk, M);

    if(checher::check_IQF(v_out, v_out_psk)){
        std::cout << "Success! " << std::endl;
        //    printer::print_IQF(v_out_psk);
    }

}

void test::test4() {

    std::cout << "Testing Task4..." << std::endl;

    std::vector<float> v_in = file::binput_T<float>("../source/Task4/v_in_float_Real.flt");

    int ans = 18;
    float val = (float)(*std::max_element(v_in.begin(), v_in.end()) - 6.0f);
    std::cout <<"Val -> " << val << std::endl;

    int returned_val = count_more_than<float>(v_in, val);
    std::cout <<"Count more than returns -> "<< returned_val << std::endl;

    if(returned_val == ans) {
        std::cout << "Success! " << std::endl;
    }
    else{
        throw std::runtime_error("Answer and returned value are not the same!\n");
    }
}

void test::test5() {

    std::cout << "Testing Task5..." << std::endl;

    std::vector<float> v_in = file::binput_T<float>("../source/Task5/v_in_float_Real.flt");
    std::vector<int> idx_in = file::binput_T<int>("../source/Task5/idx_peak_int_Real.flt");

    float level = 6.0f;
    size_t len_win = 640;

    //Implement find_local_peaks method
    std::vector<size_t> idx;
    find_local_peaks(v_in, idx, len_win, level);

    checher::check_idx(v_in, idx, idx_in);

//    if(checher::check_result(idx, idx_in)){
//        std::cout << "Success! " << std::endl;
//        //    printer::print_T<size_t>(idx);
//    }
}