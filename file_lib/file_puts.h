//
// Created by artem on 12.03.2021.
//

#ifndef STC_FILE_PUTS_H
#define STC_FILE_PUTS_H

#include<types.hpp>
#include <string>
#include <vector>

/*
 * Функции считывания и записи в бинарные файлы
 */

std::vector<float>  binput_f(const std::string &file_input);

std::vector<IQF> binput_IQF(const std::string &file_input);

void binout(const std::vector<float> &demodulated_signal, const std::string &file_output);

#endif //STC_FILE_PUTS_H
