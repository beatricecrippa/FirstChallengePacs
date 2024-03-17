#ifndef DATA_H
#define DATA_H

#include "muparser_fun.hpp"
#include "muparser_dfun.hpp"

#include <vector>
#include <functional>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>

#include "json.hpp"
using json = nlohmann::json;

// Function to compute the gradient using finite differences
std::vector<double> GradientFD(std::vector<double> x, double h, std::function<double(std::vector<double>)> fun);

// Function to compute step size using Armijo rule
double Armijo(double a0, const double mu,const unsigned int k, const std::vector<double>& xk,
            std::function<double(std::vector<double>)> fun, std::function<std::vector<double>(std::vector<double>)> dfun);

// Data structure to hold input parameters
struct Data{
    std::vector<double> x0; // Initial guess
    unsigned int max_it;    // Maximum number of iterations
    double tol_x;           // Tolerance for the change in x
    double tol_df;          // Tolerance for the change in the function value
    std::function<double(const unsigned int, std::vector<double>)> Alpha; // Step size function

    int method;             // Optimization method
    double gamma1;          // Hyperparameter for Adam
    double gamma2;          // Hyperparameter for Adam
    double epsilon;         // Hyperparameter for Adam
    double eta;             // Hyperparameter for Heavy Ball

    std::function<double(std::vector<double>)> fun; // Objective function
    std::function<std::vector<double>(std::vector<double>)> Grad; // Gradient function

    // Constructor to initialize parameters from JSON file
    Data(const std::string& filename) {

        std::ifstream f(filename);
        json data = json::parse(f);

        // Initializing parameters from JSON data
        std::vector<double> a= data["initial"];
        x0=a;
        max_it = data.value("max_it",100);
        tol_x = data.value("tol_x", 1e-7);
        tol_df = data.value("tol_df", 1e-7);

        method = data.value("learningMethod", 0);
        gamma1 = data.value("gamma1AD", 0.1);
        gamma2 = data.value("gamma2AD", 0.1);
        epsilon= data.value("epsilonAD", 1e-7);
        eta = data.value("etaHB", 0.1);

        // Initializing objective function and its gradient
        std::string funString = data.value("fun","");
        std::vector<std::string> dfunVstring = data["dfun"];        
        MuparserFun muFun(funString, x0.size());
        fun = muFun;
        MuparserDfun dfun(dfunVstring, x0.size());

        // Checking if finite differences should be used for gradient computation
        int finiteDifferences = data.value("finiteDifferences", 0);
        double h = data.value("hFD", 1e-5);
        if(finiteDifferences==1){
            Grad=[h, this](std::vector<double> x){ return GradientFD(x, h,fun);};
            std::cout<<"Using finite differences for Gradient evaluation"<<std::endl;
        }
        else{
            Grad = dfun;
        }

        // Initializing step size function based on chosen method
        double a0 = data.value("alpha0", 0.1);
        double mu = data.value("decayParameter", 0.1);
        int alphaMethod = data.value("alphaMethod", 0);

        if (alphaMethod == 0) {
            Alpha=[a0](const unsigned int k, std::vector<double> x){ return a0;};
        } else if (alphaMethod == 1) {
            Alpha = [a0,mu](const unsigned int k, std::vector<double> x){ return a0*std::exp(-mu*k);};
        } else if (alphaMethod == 2) {
            Alpha = [a0,mu](const unsigned int k, std::vector<double> x){ return a0/(1+mu*k);};
        } else if (alphaMethod == 3) {
            Alpha = [a0,mu,this](const unsigned int k, std::vector<double> x){ return Armijo(a0,mu,k,x,fun,Grad);};
            if(method==2 or method==3)
                std::cerr << "Cannot use Armijo method with Heavy ball and Adam, please change method." << std::endl;
        } else {
            std::cerr << "Invalid choice of alphaMetod, insert a value between 0 and 3" << std::endl;
        }
    }

};

// Function to compute gradient using finite differences
std::vector<double> GradientFD(std::vector<double> x, double h, std::function<double(std::vector<double>)> fun){
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

// Function to compute the norm of a vector
double norm(std::vector<double> x){
    double norm=0;
    for(size_t i=0; i<x.size(); ++i){
        norm += std::pow(x[i],2);
    }
    return std::pow(norm,0.5);
}

// Operator overloading for vector addition
std::vector<double> operator +(std::vector<double> x, std::vector<double> y){
    for(size_t i=0; i<x.size(); ++i){
        x[i]+=y[i];
    }
    return x;
}

// Operator overloading for vector subtraction
std::vector<double> operator -(std::vector<double> x, std::vector<double> y){
    for(size_t i=0; i<x.size(); ++i){
        x[i]-=y[i];
    }
    return x;
}

// Operator overloading for scalar-vector multiplication
std::vector<double> operator *(double a, std::vector<double> V){
    for(size_t i=0; i<V.size(); ++i){
        V[i]*=a;
    }
    return V;
}

// Function to compute step size using Armijo rule
double Armijo(double a0, const double mu,const unsigned int k, const std::vector<double>& xk, std::function<double(std::vector<double>)> fun, std::function<std::vector<double>(std::vector<double>)> dfun){

    while(fun(xk) - fun(xk- (a0*dfun(xk))) < mu*a0*norm(dfun(xk))*norm(dfun(xk))){
        a0=a0/2;
    }
    return a0;
}

#endif // DATA_H