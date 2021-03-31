//
// Created by artem on 10.03.2021.
//
#ifndef STC_PSK_MOD_H
#define STC_PSK_MOD_H

#include <types.hpp>

#include <vector>
#include <iostream>
#include <cmath>

static const float TWO_PI = 2.0f * (float)M_PI;

void psk_mod(const std::vector<int> &v_in, std::vector<IQF> &v_out , size_t M){
    bool f = M && !(M & (M - 1));

    //Проверка, что M степень двойки
    if(!f) {
        throw std::runtime_error("M is not a power of two!\n");
    }

    //Проверка, что значения v_in лежат в пределах 0..M-1 за O(n)
    for(auto i : v_in){
        if ((i < 0) | (i > (int)(M - 1)))
            throw std::runtime_error("element " + std::to_string(i) + " is out of [0.." + std::to_string(M-1) + "]\n");
    }

    if(v_out.empty()) {
        v_out.resize(v_in.size());
    }

    for (size_t i = 0; i < v_in.size(); ++i){
        float theta = (TWO_PI  * v_in[i]) / M;
        v_out[i] = IQF(theta);
    }
}

#endif //STC_PSK_MOD_H