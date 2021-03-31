//
// Created by artem on 13.03.2021.
//

#ifndef STC_PRINT_HPP
#define STC_PRINT_HPP

#include <types.hpp>
#include <vector>

using namespace types;

namespace printer {

    void print_IQF(const std::vector<IQF> &vec);

    template<typename T>
    void print_T(const std::vector<T> &vec);

    template<typename T, typename Q>
    void compare(const std::vector<T> &vec1, const std::vector<Q> &vec2);

}
#endif //STC_PRINT_HPP