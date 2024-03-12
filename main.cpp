// MAIN

// To run:     g++ Data.hpp main.cpp -o main      ./main

#include "Data.hpp"
#include <iostream>
#include "GradientDescent.cpp"

int main(){
    Data data({0,0}, 0.000001,0.0000001, 10000, [](Array2D x){return x[0]*x[0] +2;}, [](Array2D x){ Array2D r={2*x[0],0};  return r;});

    std::cout << GradientDescent(data)<< std::endl;

    return 0;
}

