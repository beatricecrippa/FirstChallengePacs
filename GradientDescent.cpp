#include "Data.hpp"
#include <cmath>
#include <iostream>

double norm(Array2D x){
    return x[0]*x[0] + x[1]*x[1];
}

Array2D operator + (double const a, Array2D const x){
    return {a+x[0],a+x[1]};
}
Array2D operator - (double const a, Array2D const x){
    return {a-x[0],a-x[1]};
}
Array2D operator * (double const a, Array2D const x){
    return {a*x[0],a*x[1]};
}

Array2D operator - (Array2D const x, Array2D const y){
    return {x[0]-y[0],x[1]-y[1]};
}

double GradientDescent(Data data){
    Array2D xk = data.x0;
    Array2D x;
    int it=0;
    double alpha=0.1;
    
    while(true){
    x= xk - (alpha* data.Df(xk));
    it++;

    if(norm(x)<data.tol or norm(data.Df(xk)) < data.res or it> data.max_it)
        return data.f(x);
    }
}