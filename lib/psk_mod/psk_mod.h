//
// Created by artem on 10.03.2021.
//

#ifndef STC_PSK_MOD_H
#define STC_PSK_MOD_H

#include <vector>
#include <cmath>

struct IQF{
public:
    IQF(float i1, float q1) : i(i1), q(q1){};

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

void psk_mod(const std::vector<int> &v_in, std::vector<IQF> &v_out , size_t M);


#endif //STC_PSK_MOD_H
