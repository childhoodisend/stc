//
// Created by artem on 10.03.2021.
//

#include "count_more_than.h"

#include <vector>
#include <iostream>


template <typename T>
int count_more_than (const std::vector<T>& v_in, T val){
    if(!v_in.empty()) {
        int counter = 0;
        for(auto i : v_in)
            counter = (i > val) ? counter + 1 : counter;

        return counter;
    }
    else
        throw std::runtime_error("input vector is empty!\n");
}