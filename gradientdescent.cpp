#include "gradientdescent.hpp"
#include <functional>

#include <cmath>

std::vector<double> operator -(std::vector<double> x, std::vector<double> y);
std::vector<double> operator *(double a, std::vector<double> V);

double norm(std::vector<double> x);

double Armijo(double a0, const double mu,const unsigned int k, const std::vector<double>& xk,
            std::function<double(std::vector<double>)> fun, std::function<std::vector<double>(std::vector<double>)> dfun);



std::vector<double> GradientDescent(const Data & data){

    const std::vector<double>& initial_x=data.x0;
    const unsigned int max_it=data.max_it;
    const double tol_df=data.tol_df;
    const double tol_x=data.tol_x;
    std::function<double(std::vector<double>)> fun=data.fun;
    std::function<std::vector<double>(std::vector<double>)> dfun=data.Grad;
    const double a0=data.alpha0;
    const double mu=data.decayParameter;
    const int method=data.learningMethod;

    
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
        //std::cout<<a<<std::endl;
        std::vector<double> DF=dfun(xk);

        xk1=xk-(a*DF);
        
        if(it > max_it or norm(xk-xk1)<tol_x or norm(dfun(xk)) < tol_df)
            converged=true;
        xk=xk1;
        ++it;
    }

    return xk;
}



double Armijo(double a0, const double mu,const unsigned int k, const std::vector<double>& xk, std::function<double(std::vector<double>)> fun, std::function<std::vector<double>(std::vector<double>)> dfun){
    
    while(fun(xk) - fun(xk- (a0*dfun(xk))) < mu*a0*norm(dfun(xk))*norm(dfun(xk)) ){
        a0=a0/2;
    }
    return a0;
}




double norm(std::vector<double> x){
    double norm=0;
    for(size_t i=0; i<x.size(); ++i){
        norm += std::pow(x[i],2);
    }
    return std::pow(norm,0.5);
}

std::vector<double> operator -(std::vector<double> x, std::vector<double> y){
    for(size_t i=0; i<x.size(); ++i){
        x[i]-=y[i];
    }
    return x;
}
std::vector<double> operator *(double a, std::vector<double> V){
    for(size_t i=0; i<V.size(); ++i){
        V[i]*=a;
    }
    return V;
}