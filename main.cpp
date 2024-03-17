#include "muparser_fun.hpp"
#include "muparser_dfun.hpp"
#include "gradientdescent.hpp"
#include "Data.hpp"

#include <string>
#include <vector>
#include <iostream>




int main(){

    Data data("data.json");

    std::vector<double> final_x = GradientDescent(data);

    std::cout << final_x[0] << " " << final_x[1] << std::endl;

    return 0;
}


#include "gradientdescent.cpp"    //DA CAMBIARE CHAT CREDO RIESCA