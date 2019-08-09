/*************************************************************************
 > File Name: test.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Thu 08 Aug 2019 07:51:04 PM PDT
 ************************************************************************/

#include <hiredis/hiredis.h>
#include<iostream>

using namespace std;
int main(int argc,char* argv[]){
	redisContext *c;
	redisReply *reply;

	const char*hostname = argc>1?argv[1]:"127.0.0.1";
	int port = argc>2?atoi(argv[2]):6379;
	struct timeval timeout = {1,500000};

	c = redisConnectWithTimeout(hostname,port,timeout);
	if(!c || c->err){
		if(!c){
			cout << "Conncetion error:can't allocate redis context" << endl;
		}else{
			cout << "Conncetion error:" << c->err << endl;
		}
		exit(1);
	}

	// ping server
	reply = (redisReply*)redisCommand(c,"ping");
	cout << "ping:" << reply->str << endl;
	freeReplyObject(reply);

	//set a key
	reply = static_cast<redisReply*>(redisCommand(c,"set %s %s","foo","bar"));
	cout << "set:" << reply->str << endl;
	freeReplyObject(reply);

	//get a key
	reply = static_cast<redisReply*>(redisCommand(c,"get %s","foo"));
	cout << "get foo:" << reply->str << endl;
	freeReplyObject(reply);

	reply = static_cast<redisReply*>((redisCommand(c,"incr num")));
	cout << "incr num:" << reply->integer << endl;
	freeReplyObject(reply);
	//-------------get num一直是0  为什么？因为返回的是一个字符串，不能用integer来获取数据
	reply = static_cast<redisReply*>(redisCommand(c,"get num"));
	cout << "get num:" << reply->str << endl;
	//错误用法
	//cout << "get num:" << reply->integer << endl;
	freeReplyObject(reply);

	//------------一个查询空即key的返回结果是
	reply = static_cast<redisReply*>(redisCommand(c,"get sun"));
	if(reply->type!=REDIS_REPLY_STRING){
		cout << "nullptr" << endl;
		cout << reply->type << endl;
	}
	cout << "get sun:" << reply->str << endl;
	cout << "get sun:len = " << reply->len << endl;
	freeReplyObject(reply);
	redisFree(c);
}

