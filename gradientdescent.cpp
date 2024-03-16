#include "gradientdescent.hpp"
#include <functional>

#include <cmath>

std::vector<double> diff(std::vector<double> x, std::vector<double> y){

    for(size_t i=0; i<x.size(); ++i){
        x[i]-=y[i];
    }
    return x;
}

double norm(std::vector<double> x){
    double norm=0;

    for(size_t i=0; i<x.size(); ++i){
        norm += std::pow(x[i],2);
    }
    return std::pow(norm,0.5);
}

std::vector<double> prodScal(double a, std::vector<double> V){
    for(size_t i=0; i<V.size(); ++i){
        V[i]*=a;
    }
    return V;
}


double Armijo(double a0, const double mu,const unsigned int k, const std::vector<double>& xk, MuparserFun fun, MuparserDfun dfun){
    
    while(fun(xk) - fun(diff(xk,prodScal(a0,dfun(xk)))) < mu*a0*norm(dfun(xk))*norm(dfun(xk)) ){
        a0=a0/2;
    }

    return a0;
}

std::vector<double> GradientDescent(const std::vector<double>& initial_x, 
                                    const unsigned int max_it, 
                                    const double tol_df, 
                                    const double tol_x, 
                                    MuparserFun fun, 
                                    MuparserDfun dfun,
                                    const double a0,
                                    const double mu,
                                    const int method){
    
    unsigned int it =0;
    std::vector<double> xk(initial_x);
    bool converged=false;

    std::vector<std::function<double(const unsigned int)>> Alphas;
    Alphas.push_back([a0,mu](const unsigned int k){ return a0*std::exp(-mu*k);});
    Alphas.push_back([a0,mu](const unsigned int k){ return a0/(1+mu*k);});
    Alphas.push_back([=](const unsigned int k){ return Armijo(a0,mu,k,xk,fun,dfun);});

    while(!converged){
        std::vector<double> xk1(xk.size());
        double a=Alphas[method](it);
        std::cout<<a<<std::endl;
        std::vector<double> DF=dfun(xk);

        xk1=diff(xk, prodScal(a,DF));
        
        if(it > max_it or norm(diff(xk,xk1))<tol_x or norm(dfun(xk)) < tol_df)
            converged=true;
        xk=xk1;
        ++it;
    }

    return xk;
}