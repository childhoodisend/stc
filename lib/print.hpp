//
// Created by artem on 13.03.2021.
//

#ifndef STC_PRINT_HPP
#define STC_PRINT_HPP

#include <types.hpp>
#include <vector>

namespace print{

    void print_IQF(const std::vector<IQF> &vec) {
        for (const auto &i : vec)
            std::cout << "i -> " << i.i << " q -> " << i.q << " theta -> " << i.theta << std::endl;
    }

    template<typename T>
    void print_T(const std::vector<T> &vec) {
        for (const auto &i : vec)
            std::cout << i << std::endl;
    }

}
#endif //STC_PRINT_HPP
