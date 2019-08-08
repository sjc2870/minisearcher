#pragma once
#include "WebPage.h"
#include "tinyxml2.h"
#include "cppjieba/Jieba.hpp"
#include <vector>
#include <set>
#include <string>
using std::string;
using std::set;
using std::vector;
using namespace tinyxml2;
namespace wd{
class FileProcessor{
public:
	FileProcessor(vector<string>&);
	WebPage process(XMLElement*,int id,bool);
	vector<string> separateWords(const string&);
	map<string,int> getFrequency(const vector<string>&);
	vector<string> getTopk(map<string,int>&);
	set<WebPage> create();
	void readStopWords(const string&);
private:
	vector<string>& m_filePath;
	set<string> m_stopWords;
	cppjieba::Jieba m_jieba;
};
}//end of namespace wd
