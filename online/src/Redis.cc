/*************************************************************************
 > File Name: Redis.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Thu 08 Aug 2019 11:56:11 PM PDT
 ************************************************************************/

#include "Redis.h"
#include<iostream>
using std::cout;
using std::endl;

namespace wd{

Redis* Redis::getInstance(){
	if(m_pInstance==nullptr){
		m_pInstance = static_cast<Redis*>(new Redis());
	}
	return m_pInstance;
}

void Redis::destory(){
	delete m_pInstance;
}

Redis::Redis()
:m_context()
,m_reply(){
	const char*hostname = "127.0.0.1";
	int port = 6379;
	struct timeval timeout = {1,500000};
	m_context = redisConnectWithTimeout(hostname,port,timeout);
}

Redis::~Redis(){}

string Redis::get(string msg){
	string ret;
	m_reply = static_cast<redisReply*>(redisCommand(m_context,"get %s",msg.c_str()));
	if(m_reply->type!=REDIS_REPLY_NIL){
		return m_reply->str;
	}else{
		string s;
		return s;
	}
}

void Redis::set(string key,string value){
	redisCommand(m_context,"set %s %s",key.c_str(),value.c_str());
}
}//end of namespace wd
