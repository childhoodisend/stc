//
// Created by artem on 08.03.2021.
//

#include "rectpulse.h"
#include <vector>
#include <iostream>
#include <iterator>

template <typename T>
void rectpulse(const std::vector<T>& v_in, std::vector<T>& v_out, size_t nsamp){
    if(v_in.empty()){
        throw std::runtime_error("v_in is empty!");
    }

    else {
        size_t vec_size = v_in.size();
        if(v_out.size() != nsamp*vec_size)
            v_out.resize(nsamp*vec_size);

        auto it = v_in.begin();
        for(size_t i = 0; i < v_out.size(); ++i){
            if(i % nsamp == 0 && i)
                ++it;

            v_out[i] = *it;
        }
    }
}