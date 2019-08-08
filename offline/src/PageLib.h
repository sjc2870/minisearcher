#pragma once
#include "WebPage.h"
#include "Config.h"
#include <set>
#include <unordered_map>
using std::set;
using std::unordered_map;
using std::pair;
namespace wd{

class PageLib{
public:
	PageLib(set<WebPage>&,Config&);
	void writeToFile();//将网页库和偏移库写入到磁盘文件
	void createInvertIndexTable();
	void printInvertIndexTable();
private:
	set<WebPage>& m_pageLib;
	Config &m_config;
	unordered_map<string,vector<pair<int,double>>> m_invertIndexTable;
};
}//end of namespace wd
