#pragma once
#include "cppjieba/Jieba.hpp"
#include <string>
#include <map>
#include <vector>
#include <set>
using std::set;
using std::vector;
using std::string;
using std::map;
using namespace cppjieba;
namespace wd{

class WebPage{
public:
	WebPage(const string&,Jieba&,set<string>&);
	void process(const string&,Jieba&,set<string>&);
	int getId();
	string getTitle()const;
	string getLink()const;
	string getSummary(vector<string>& msg)const;
	map<string,int>& getWordsAndFre();
	friend bool operator==(const WebPage&,const WebPage&);
	friend bool operator<(const WebPage&,const WebPage&);
private:
	vector<string> separateWords(const string&,Jieba&);
private:
	int m_id;
	string m_title;
	string m_link;
	string m_content;
	string m_summary;
	vector<string> m_topWords;
	map<string,int> m_wordsAndFre;
};
}
