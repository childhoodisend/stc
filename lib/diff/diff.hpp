//
// Created by artem on 08.03.2021.
//
#ifndef STC_DIFF_H
#define STC_DIFF_H


#include <vector>
#include <iostream>

template<typename T>
void diff(const std::vector<T> &v_in, std::vector<T> &v_out){

    if(v_in.empty()) {
        throw std::runtime_error("v_in is empty!");
    }

    else {

        size_t vec_size = v_in.size();

        if(v_out.size() != vec_size - 1)
            v_out.resize(vec_size - 1);

        for(size_t i = 0; i < vec_size - 1; ++i){
            v_out[i] = (T)(v_in[i+1] - v_in[i]);
        }
    }
}

#endif //STC_DIFF_H