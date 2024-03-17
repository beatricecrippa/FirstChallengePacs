#ifndef GRADIENTD_H
#define GRADIENTD_H

#include "muparser_fun.hpp"
#include "muparser_dfun.hpp"
#include "Data.hpp"

#include <vector>
#include <functional>

std::vector<double> GradientDescent(const Data & data);

#endif // GRADIENTD_H