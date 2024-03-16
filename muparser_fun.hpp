#ifndef FUN_MUPARSER
#define FUN_MUPARSER

#include <muParser.h>

#include <vector>
#include <memory>
#include <string>

class MuparserFun
{
public:
  MuparserFun(const MuparserFun &m)
    : m_parser(m.m_parser)
  {
    m_var.resize(m.m_var.size());
    for(size_t i=0;i<m_var.size();++i){
        m_parser.DefineVar("x"+std::to_string(i+1), &m_var[i]);
    }
  };

  MuparserFun(const std::string &s, size_t size)
  {
    m_var.resize(size);
    try
      {
        for(size_t i=0;i<size;++i){
          m_parser.DefineVar("x"+std::to_string(i+1), &m_var[i]);
        }
        m_parser.SetExpr(s);
      }
    catch (mu::Parser::exception_type &e)
      {
        std::cerr << e.GetMsg() << std::endl;
      }
  };

  double
  operator()(const std::vector<double> &x)
  {
    std::copy(x.begin(), x.end(), m_var.begin());
    double y = m_parser.Eval();
    return y;
  };

private:
  std::vector<double> m_var;
  mu::Parser m_parser;
};

#endif // FUN_MUPARSER