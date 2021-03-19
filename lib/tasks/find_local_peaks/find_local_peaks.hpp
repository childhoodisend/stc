//
// Created by artem on 10.03.2021.
//

#ifndef STC_FIND_LOCAL_PEAKS_H
#define STC_FIND_LOCAL_PEAKS_H

#include <vector>
#include <iostream>
#include <limits>

void find_local_peaks(const std::vector<float>& v_in, std::vector<size_t>& idx_peak, size_t len_win, float level){
    

/*
 * Проверки
 */
    if (v_in.empty()) {
        throw std::runtime_error("input vector is empty!\n");
    }

    if(v_in.size() < 2 * len_win) {
        throw std::runtime_error("input vector less than 2 * len_win!\n");
    }

    if(len_win <= 0) {
        throw std::invalid_argument("window length is <= 0!\n");
    }

    bool flag = false;
    float maximum = std::numeric_limits<float>::min();
    size_t index = 0;

    for(size_t i = len_win; i < v_in.size() - len_win; ++i) {

        if (!flag) {
            // Условие на начало области локального максимума:
            if ((v_in[i] - v_in[i - len_win] > level) && (v_in[i] - v_in[i + len_win] > level)) {
                flag = true;
                maximum = v_in[i];
                index = i;
                idx_peak.push_back(index);
            }
        }
        else {
            //Условие конца области локального максимума:
            if (v_in[i] - v_in[i - len_win] < 0) {
                flag = false;
                idx_peak.back() = index;
            }
        }
        if(flag){
            if(v_in[i] > maximum){
                index = i;
                maximum = v_in[i];
            }
        }
    }
}

#endif //STC_FIND_LOCAL_PEAKS_H