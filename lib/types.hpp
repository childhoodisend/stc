//
// Created by artem on 12.03.2021.
//

#ifndef STC_TYPES_HPP
#define STC_TYPES_HPP

#include <cmath>

struct IQF{
public:
    IQF(float i1, float q1) : i(i1), q(q1){
        theta = atan2f(q, i);
    };

    explicit IQF(float theta1) : theta(theta1){
        i = cosf(theta1);
        q = sinf(theta1);
    };

    IQF() = default;
    ~IQF() = default;
public:
    float theta{};
    float i{}, q{};
};


#endif //STC_TYPES_HPP
