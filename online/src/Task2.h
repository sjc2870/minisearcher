#pragma once
#include "TcpConnection.h"
#include "WordQuery.h"
#include "cppjieba/Jieba.hpp"
#include "Config.h"
#include "Redis.h"
using namespace cppjieba;
namespace wd{

class Task2{
public:
Task2(const string&,const TcpConnectionPtr&,Jieba &,WordQuery&,Redis*);
void process();
string getReturnFromRedis();
void setRedis(string value);
private:
string m_msg;
wd::TcpConnectionPtr m_conn;
Jieba &m_jieba;
WordQuery& m_wordquery;
Redis* m_pRedis;
};

}//end of namespace wd
