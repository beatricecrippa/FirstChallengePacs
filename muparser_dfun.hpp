#ifndef DFUN_MUPARSER
#define DFUN_MUPARSER

#include "muparser_fun.hpp"

#include <vector>
#include <memory>
#include <string>

class MuparserDfun
{
private:
  std::vector<MuparserFun> m_vparser;

public:
  MuparserDfun(const MuparserDfun &m) : m_vparser(m.m_vparser){};

  MuparserDfun(const std::vector<std::string> &sv, size_t size)
  {
    for(const std::string & s : sv){
      m_vparser.emplace_back(s,size);
    }
  };

  std::vector<double>
  operator()(const std::vector<double> &x)
  {
    std::vector<double> grad;
    for(MuparserFun f : m_vparser){
      grad.emplace_back(f(x));
    }
    return grad;
  };
  
};



#endif // DFUN_MUPARSER