 ///
 /// @file    TestAcceptor.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 15:42:14
 ///
 
#include "Threadpool.h"
#include "TcpServer.h"
#include "Config.h"
#include "Task2.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using wd::Task2;

wd::Threadpool * gThreadpool = nullptr;

wd::Config config("../conf/myconf.conf");
const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

	Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
 
//回调函数体现了扩展性
void onConnection(const wd::TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
//	conn->send("welcome to server.");
}

void onMessage(const wd::TcpConnectionPtr & conn)
{
	cout << "onMessage...." << endl;
	string msg = conn->receive();
	cout << ">> receive msg from client: " << msg << endl;
	//业务逻辑的处理要交给线程池处理
	//decode
	//compute
	//encode
	//::sleep(2);//碰到需要长时间的处理时，响应速度会降下来
	//conn->send(msg);
	Task2 task(msg, conn,jieba,config);

	gThreadpool->addTask(std::bind(&Task2::process, task));
}

void onClose(const wd::TcpConnectionPtr & conn)
{
	cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

int main(void)
{
#if 1
	wd::Threadpool threadpool(4, 10); 
	threadpool.start();
	
	gThreadpool = &threadpool;
#endif

#if 1
	wd::TcpServer server("192.168.4.130", 8888);

	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setCloseCallback(onClose);

	server.start();
#endif
	return 0;
}
