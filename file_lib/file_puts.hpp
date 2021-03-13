//
// Created by artem on 12.03.2021.
//

#ifndef STC_FILE_PUTS_HPP
#define STC_FILE_PUTS_HPP

#include<types.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*
 * Функции считывания и записи в бинарные файлы
 */
namespace file {

    template<typename T>
    std::vector<T> binput_T(const std::string &file_input) {
        std::fstream fin(file_input, std::ios_base::binary | std::ios_base::in);
        if (fin) {
            std::vector<T> buf;

            fin.seekg(0, std::ios_base::end);
            size_t filesize=fin.tellg();
            fin.seekg(0, std::ios_base::beg);

            buf.resize(filesize/sizeof(T)+(filesize%sizeof(T)?1U:0U));

            std::cout <<"File size -> "<< filesize  <<" bytes   "<< std::endl;
            std::cout <<"Buf  size -> "<< buf.size()<<"  elements"<< std::endl;
            fin.read((char*)buf.data(), filesize);
            fin.close();

            std::cout << "Binary file "<< file_input <<" has been read successfully!" << std::endl;
            std::cout << std::endl;
            return (std::vector<T>) buf;
        }
        else {
            throw std::invalid_argument("Invalid input file!");
        }
    }


    std::vector<IQF> binput_IQF(const std::string &file_input) {
        std::fstream fin(file_input, std::ios_base::binary | std::ios_base::in);
        if (fin) {
            std::vector<float> buf;

            fin.seekg(0, std::ios_base::end);
            size_t filesize=fin.tellg();
            fin.seekg(0, std::ios_base::beg);

            buf.resize(filesize/sizeof(float)+(filesize%sizeof(float)?1U:0U));
            if (buf.size() % 2 != 0)
                throw std::runtime_error("buffer size is odd");

            std::cout <<"File size -> "<< filesize  <<" bytes   "<< std::endl;
            std::cout <<"Buf  size -> "<< buf.size()<<"  elements"<< std::endl;
            fin.read((char*)buf.data(), filesize);
            fin.close();


            std::vector<IQF> complex_signal;
            complex_signal.reserve(buf.size() / 2);

            IQF comp{};
            for (size_t i = 0; i < buf.size() - 1; i += 2) {
                comp.i = buf[i];
                comp.q = buf[i + 1];
                comp.compute_theta();

                complex_signal.push_back(comp);
            }


            std::cout << "Binary file "<< file_input <<" has been read successfully!" << std::endl;
            std::cout << std::endl;
            return (std::vector<IQF>) complex_signal;

        }
        else {
            throw std::invalid_argument("Invalid input file!");
        }
    }

    void binout(const std::vector<float> &src, const std::string &file_output) {
        std::ofstream fout(file_output, std::ios_base::binary | std::ios_base::out);
        if (fout) {
            for (auto i : src) {
                fout.write((char *) &i, sizeof(i));
            }
            fout.close();
            std::cout << "Written to binary file "<< file_output <<" successfully!" << std::endl;
        }
        else {
            throw std::invalid_argument("Invalid output file!");
        }
    }


}
#endif //STC_FILE_PUTS_HPP
