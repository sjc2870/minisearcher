#pragma once
#include "WebPage.h"
#include "Config.h"
#include <unordered_map>
using std::unordered_map;
using std::pair;
namespace wd{

class WordQuery{
public:
	WordQuery(Config&,Jieba&);
	void loadFile();
	string query(const string&);
private:
	void readStopWords();
	string toJson(vector<int>&,vector<string>&);
	vector<int> sortPages(set<int>&,vector<string>&);
	string notFound();
private:
	vector<WebPage> m_pageLib;
	Config& m_config;
	Jieba& m_jieba;
	set<string> m_stopWords;
	unordered_map<int,pair<int,int>> m_offsetLib;
	unordered_map<string,vector<pair<int,double>>> m_invertIndexTable;
};
}
