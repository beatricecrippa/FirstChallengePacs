#include "muparser_fun.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;
#include <string>

#include <iostream>

int main(){
    std::ifstream f("data.json");
    json data = json::parse(f);


    const double x0 = data.value("initial", 1);
    const unsigned int max_it =  data.value("max_it",100);
    const double tol_res = data.value("tol_res", 1e-7);
    const double tol_x = data.value("tol_x", 1e-7);
    std::string funString = data.value("fun","");
    std::string dfunString = data.value("dfun","");

    MuparserFun fun(funString);
    MuparserFun dfun(dfunString);

    std::cout << fun(x0) << std::endl;

    return 0;
}
