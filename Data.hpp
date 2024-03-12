#ifndef DATA_H
#define DATA_H

#include <array>
#include <functional>

typedef std::array<double, 2> Array2D;

// Define the struct

struct Data{
    // initial conditions or starting point
    Array2D x0; 
    // tolerances
    double tol; // control of step length
    double res; // control on residual 
    // initial step ???
    // maximal number of iterations
    int max_it;
    // function wrapper f
    std::function<double(Array2D)> f;
    // function wrapper for gradient of f
    std::function<Array2D(Array2D)> Df;

    // Constructor, to easily initialize the struct
    Data(Array2D x0, double tol, double res, int max_it, std::function<double(Array2D)> f,
        std::function<Array2D(Array2D)> Df) : x0(x0), tol(tol), res(res), max_it(max_it), f(f), Df(Df) {};
};

#endif // DATA_H
