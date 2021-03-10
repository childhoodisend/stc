//
// Created by artem on 10.03.2021.
//

#include "find_local_peaks.h"

#include <vector>
#include <iostream>

void find_local_peaks(const std::vector<float>& v_in, std::vector<size_t>& idx_peak, size_t len_win, float level){
    

/*
 * Проверки
 */
    if (v_in.empty())
        throw std::runtime_error("input vector is empty!\n");
    if(len_win <= 0)
        throw std::invalid_argument("window length is <= 0!\n");

}