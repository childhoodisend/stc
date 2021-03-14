//
// Created by artem on 13.03.2021.
//

#ifndef STC_PRINT_HPP
#define STC_PRINT_HPP

#include <types.hpp>
#include <vector>

namespace printer{

    void print_IQF(const std::vector<IQF> &vec) {
        for (const auto &i : vec)
            std::cout << "i -> " << i.i << " q -> " << i.q << " theta -> " << i.theta << std::endl;
    }

    template<typename T>
    void print_T(const std::vector<T> &vec) {
        for (const auto &i : vec)
            std::cout << i << std::endl;
    }

    template<typename T, typename Q>
    void compare(const std::vector<T> &vec1, const std::vector<Q> &vec2){
        if(vec1.size() != vec2.size()) {
            std::cerr << "Bad sizes" << std::endl;
        }
        const size_t n = std::min(vec1.size(),vec2.size());

        for(size_t i = 0; i < n; ++i){
            std::cout << vec1[i] << " " << vec2[i] << std::endl;
        }

    }

}
#endif //STC_PRINT_HPP
