#pragma once
#include <string>
#include <hiredis/hiredis.h>
using namespace std;
namespace wd{

class Redis{
public:
	string get(string);
	void set(string,string);
	static Redis* getInstance();
	static void destory();
private:
	Redis();
	~Redis();
private:
	static Redis* m_pInstance;
	redisContext *m_context;
	redisReply* m_reply;
};
}
