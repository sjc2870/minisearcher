/*************************************************************************
 > File Name: Task2.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Tue 06 Aug 2019 06:02:47 PM PDT
 ************************************************************************/

#include "Task2.h"
#include<iostream>
using std::cout;
using std::endl;

const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

namespace wd{
	Task2::Task2(const string&msg,const wd::TcpConnectionPtr& conn,Jieba& jieba,Config&config)
	:m_msg(msg)
	,m_conn(conn)
	,m_jieba(jieba)
	,m_config(config)
	,m_wordquery(m_config,m_jieba){}

	void Task2::process(){
		string responce = m_wordquery.query(m_msg);
		cout << "-------------------放字符串内容为--------------------" << endl;
		cout << responce << endl;
		string ret = std::to_string(responce.size());
		responce = ret+'\n'+responce+'\n';
		m_conn->sendInLoop(responce);
	}

}//end of namespace wd
