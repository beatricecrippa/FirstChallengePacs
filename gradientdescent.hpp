#ifndef GRADIENTD_H
#define GRADIENTD_H

#include "muparser_fun.hpp"
#include "muparser_dfun.hpp"
#include <vector>

std::vector<double> GradientDescent(const std::vector<double>& initial_x, 
                                    const unsigned int max_it, 
                                    const double tol_df, 
                                    const double tol_x, 
                                    MuparserFun fun, 
                                    MuparserDfun dfun,
                                    const double a0,
                                    const double mu,
                                    const int method);

#endif // GRADIENTD_H