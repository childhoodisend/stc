//
// Created by artem on 13.03.2021.
//

#ifndef STC_CHECKER_HPP
#define STC_CHECKER_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <types.hpp>

/*
 * Checker - методы сравнивания полученного вектора
 * путем применения метода и вектора из файла *v_out*
 */


namespace checher{

    extern const float EPS = (float)1e-5;

    /*
     * Сравнивает два вектора типа T
     * Применима для float и double
     */
    template<typename T>
    bool check(const std::vector<T>& vec1, const std::vector<T>& vec2){

        if(vec1.size() != vec2.size()) {
            throw std::runtime_error("checher::check Error! Sizes vec1 and vec2 are different!");
        }

        size_t n = std::min(vec1.size(), vec2.size());

        for(size_t i = 0; i < n; ++i){
            if (std::abs(vec1[i] - vec2[i]) > EPS){
                std::cout << vec1[i] << " " <<  vec2[i] << std::endl;
                throw std::runtime_error("checher::check Error at " + std::to_string(i) + " position!\n");
            }
        }
        return (bool)true;
    }


    /*
     * Сравнивает два вектора типа IQF с заданными полями
     */

    bool check_IQF(const std::vector<IQF>& vec1, const std::vector<IQF>& vec2){

        if(vec1.size() != vec2.size()) {
            throw std::runtime_error("checher::check_IQF Error! Sizes vec1 and vec2 are different!");
        }

        size_t n = std::min(vec1.size(), vec2.size());

        for(size_t i = 0; i < n; ++i){
            if(std::abs(vec1[i].i - vec2[i].i) > EPS || std::abs(vec1[i].q - vec2[i].q) > EPS){
                throw std::runtime_error("checher::check_IQF Error! Bad " + std::to_string(i) + " position!\n");
            }
        }
        return (bool)true;
    }


}



#endif //STC_CHECKER_HPP
