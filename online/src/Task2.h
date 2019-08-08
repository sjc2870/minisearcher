#pragma once
#include "TcpConnection.h"
#include "WordQuery.h"
#include "cppjieba/Jieba.hpp"
#include "Config.h"
using namespace cppjieba;
namespace wd{

class Task2{
public:
Task2(const string&,const TcpConnectionPtr&,Jieba &,Config&);
void process();
private:
string m_msg;
wd::TcpConnectionPtr m_conn;
Jieba &m_jieba;
Config &m_config;
WordQuery m_wordquery;
};

}//end of namespace wd
