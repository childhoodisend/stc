//
// Created by artem on 06.03.2021.
//

#include <iostream>
#include <vector>

#include <find_local_peaks.cpp>



//template<typename T>
//void print(const std::vector<T> &vec){
//    for(auto i : vec)
//        std::cout << i << std:: endl;
//}


//void print(const std::vector<IQF> &vec){
//    for(auto i : vec)
//        std::cout << i.theta << std:: endl;
//}


int main(){
    std::vector<float> v_in = {1.f, 2.f, 3.1f, 4.f, 5.f, 6.3f, 7.f};

    std::cout  << count_more_than(v_in, 4.1f);


    return EXIT_SUCCESS;
}