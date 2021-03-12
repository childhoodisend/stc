//
// Created by artem on 12.03.2021.
//

#include "file_puts.h"
#include<string>
#include<vector>
#include<fstream>
#include<iostream>

/*
 * Функции считывания и записи в бинарные файлы
 */

std::vector<float> binput_f(const std::string &file_input) {
    std::fstream fin(file_input, std::ios_base::binary | std::ios_base::in);
    if (fin) {
        std::vector<float> buf((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
        fin.close();

        std::cout << "Binary file has been read successfully!" << std::endl;

        return (std::vector<float>) buf;
    }
    else {
        throw std::invalid_argument("Invalid input file!");
    }
}



std::vector<IQF> binput_IQF(const std::string &file_input) {
    std::fstream fin(file_input, std::ios_base::binary | std::ios_base::in);
    if (fin) {
        std::vector<float> buf((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
        fin.close();

        if(buf.size() % 2 != 0){
            throw std::runtime_error("buffer size is odd");
        }
        else {
            std::vector<IQF> complex_signal;
            IQF comp{};
            for (size_t i = 0; i < buf.size() - 1; i += 2) {
                comp.i = buf[i];
                comp.q = buf[i + 1];
                complex_signal.push_back(comp);
            }

            std::cout << "Binary file has been read successfully!" << std::endl;

            return (std::vector<IQF>) complex_signal;
        }
    }
    else {
        throw std::invalid_argument("Invalid input file!");
    }
}

void binout(const std::vector<float> &demodulated_signal, const std::string &file_output) {
    std::ofstream fout(file_output, std::ios_base::binary | std::ios_base::out);
    if (fout) {
        for (auto i : demodulated_signal) {
            fout.write((char *) &i, sizeof(i));
        }
        fout.close();
        std::cout << "Written to binary file successfully!" << std::endl;
    }
    else {
        throw std::invalid_argument("Invalid output file!");
    }
}