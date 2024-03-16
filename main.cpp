#include "muparser_fun.hpp"
#include "muparser_dfun.hpp"
#include "gradientdescent.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;
#include <string>
#include <vector>
#include <iostream>

int main(){

    // Read data from the json file

    std::ifstream f("data.json");
    json data = json::parse(f);

    std::vector<double> initial_x = data["initial"];
    const unsigned int max_it =  data.value("max_it",100);
    const double tol_df = data.value("tol_df", 1e-7);
    const double tol_x = data.value("tol_x", 1e-7);
    const double alpha0 = data.value("alpha0", 0.1);
    const double decayParameter = data.value("decayParameter", 0.1);
    const int learningMethod = data.value("learningMethod", 0);

    std::string funString = data.value("fun","");
    std::vector<std::string> dfunVstring = data["dfun"];

    MuparserFun fun(funString, initial_x.size());
    MuparserDfun dfun(dfunVstring, initial_x.size());

    // std::cout << fun({1,2}) << std::endl;

    // std::cout <<initial_x[0] << " " << dfun(initial_x)[1] << " " << fun(initial_x) << std::endl;

    std::vector<double> final_x = GradientDescent(initial_x, max_it, tol_df, tol_x, fun, dfun, alpha0, decayParameter, learningMethod);

    std::cout << final_x[0] << " " << final_x[1] << std::endl;


    return 0;
}

#include "gradientdescent.cpp"    //DA CAMBIARE CHAT CREDO RIESCA