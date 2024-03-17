#ifndef DATA_H
#define DATA_H

#include "muparser_fun.hpp"

#include <vector>
#include <functional>
#include <fstream>
#include <string>
#include <iostream>

#include "json.hpp"
using json = nlohmann::json;

std::vector<double> GradientFD(std::vector<double> x, double h, std::function<double(std::vector<double>)> fun);

struct Data{
    std::vector<double> x0;
     unsigned int max_it;
     double tol_x;
     double tol_df;

     double alpha0;
     double decayParameter;

     int learningMethod;
     int finiteDifferences;
     double h;

    std::function<double(std::vector<double>)> fun;
    std::function<std::vector<double>(std::vector<double>)> Grad;


    // constructor, from json
    Data(const std::string& filename) {

        std::ifstream f(filename);
        json data = json::parse(f);

        std::vector<double> a= data["initial"];
        x0=a;
        max_it =  data.value("max_it",100);
        tol_x = data.value("tol_x", 1e-7);
        tol_df = data.value("tol_df", 1e-7);
        alpha0 = data.value("alpha0", 0.1);
        decayParameter = data.value("decayParameter", 0.1);
        learningMethod = data.value("learningMethod", 0);
        finiteDifferences = data.value("finiteDifferences", 0);
        h = data.value("h", 1e-5);

        std::string funString = data.value("fun","");
        std::vector<std::string> dfunVstring = data["dfun"];        
        MuparserFun muFun(funString, x0.size());
        MuparserDfun dfun(dfunVstring, x0.size());

        fun = muFun;

        if(finiteDifferences==1){
            double h_value=h;
            const auto& fun_ref = fun;
            Grad=[h_value, &fun_ref](std::vector<double> x){ return GradientFD(x, h_value,fun_ref);};
            std::cout<<"Using finite differences for Gradient evaluation"<<std::endl;
        }
        else{
            Grad = dfun;
        }
    }


};


std::vector<double> GradientFD(std::vector<double> x, double h, std::function<double(std::vector<double>)> fun){
    // Compute partial derivatives using central differences
    std::vector<double> grad;
    for(size_t i=0; i<x.size(); ++i){
        std::vector<double> xp(x);
        std::vector<double> xm(x);
        xp[i]+=h;
        xm[i]-=h;
        grad.emplace_back( (fun(xp)-fun(xm))/(2*h) );
    }
    return grad;
}

#endif // DATA_H