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

    template<typename T, typename Q, typename N>
    void check_idx(const std::vector<T>& v_in, const std::vector<Q>& idx, const std::vector<N>& idx_perfect){

        // Проверим, что векторы одного размера
        if(idx.size() != idx_perfect.size()) {
            std::cout <<"vec1 size -> " << idx.size() << " vec2 size -> " << idx_perfect.size() << std::endl;
            throw std::runtime_error("checker::check_idx Error! Sizes vec1 and vec2 are different!");
        }

        //Проверим, что индексы имеют тип int или size_t
        bool b1 = std::is_same<Q, int>::value || std::is_same<Q, size_t>::value;
        bool b2 = std::is_same<N, int>::value || std::is_same<N, size_t>::value;

        if(!(b1 & b2)){
            throw std::runtime_error("checker::check_idx Error, bad types!\n");
        }

        const size_t size = idx.size();
        size_t perfect_counter = 0;
        size_t counter = 0;
        for(size_t i = 0; i < size; ++i){

            if((v_in[idx[i] - 1] >= v_in[idx[i]]) || (v_in[idx[i]] <= v_in[idx[i] + 1])) {
                ++counter;
                std::cerr << "Wrong index " << idx[i] << std::endl;
            }
            if((v_in[idx_perfect[i] - 1] >= v_in[idx_perfect[i]]) || (v_in[idx_perfect[i]] <= v_in[idx_perfect[i] + 1])) {
                std::cerr << "Wrong perfect index " << idx_perfect[i] << std::endl;
                ++perfect_counter;
            }
//            std::cout << std::endl;
//            std::cout << v_in[idx[i] - 1] <<" "<< v_in[idx[i]]<<" "<<v_in[idx[i] + 1] << std::endl;
//            std::cout << v_in[idx_perfect[i] - 1] <<" "<< v_in[idx_perfect[i]]<<" "<<v_in[idx_perfect[i] + 1] << std::endl;
        }

        if (perfect_counter && perfect_counter < size){
            std::cerr << "Some of perfect indexes are wrong! " << std::endl;
        }
        if (perfect_counter == size){
            std::cerr << "All perfect indexes are wrong! " << std::endl;
        }
        if(counter && counter < size){
            std::cerr << "Some of indexes are wrong! " << std::endl;
        }
        if(counter == size){
            std::cerr << "All indexes are wrong! " << std::endl;
        }
    }

    template<typename T, typename Q>
    bool check_result(const std::vector<T>& vec1, const std::vector<Q>& vec2){

        if(vec1.size() != vec2.size()) {
            std::cout <<"vec1 size -> " << vec1.size() << " vec2 size -> " << vec2.size() << std::endl;
            throw std::runtime_error("checker::check_result Error! Sizes vec1 and vec2 are different!");
        }

        const size_t n = std::min(vec1.size(), vec2.size());

        for(size_t i = 0; i < n; ++i){
            if (std::abs((Q)vec1[i] - vec2[i]) > EPS){
                std::cout << vec1[i] << " " <<  vec2[i] << std::endl;
                throw std::runtime_error("checker::check_result Error at " + std::to_string(i) + " position!\n");
            }
        }
        return true;

    }

    /*
   * Сравнивает два вектора типа T
   * Применима для float и double
   */

    template<typename T>
    bool check_result(const std::vector<T>& vec1, const std::vector<T>& vec2){

        if(vec1.size() != vec2.size()) {
            std::cout <<"vec1 size -> " << vec1.size() << " vec2 size -> " << vec2.size() << std::endl;
            throw std::runtime_error("checker::check_result Error! Sizes vec1 and vec2 are different!");
        }

        const size_t n = std::min(vec1.size(), vec2.size());

        for(size_t i = 0; i < n; ++i){
            if (std::abs(vec1[i] - vec2[i]) > EPS){
                std::cout << vec1[i] << " " <<  vec2[i] << std::endl;
                throw std::runtime_error("checker::check_result Error at " + std::to_string(i) + " position!\n");
            }
        }
        return true;
    }


    /*
     * Сравнивает два вектора типа IQF с заданными полями
     */

    bool check_IQF(const std::vector<IQF>& vec1, const std::vector<IQF>& vec2){

        if(vec1.size() != vec2.size()) {
            std::cout <<"vec1 size -> " << vec1.size() << " vec2 size -> " << vec2.size() << std::endl;
            throw std::runtime_error("checker::check_IQF Error! Sizes vec1 and vec2 are different!");
        }

        const size_t n = std::min(vec1.size(), vec2.size());

        for(size_t i = 0; i < n; ++i){
            if(std::abs(vec1[i].i - vec2[i].i) > EPS || std::abs(vec1[i].q - vec2[i].q) > EPS){
                throw std::runtime_error("checker::check_IQF Error! Bad " + std::to_string(i) + " position!\n");
            }
        }
        return true;
    }


}



#endif //STC_CHECKER_HPP
