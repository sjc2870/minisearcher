#pragma once
#include <string>
#include <vector>
#include <map>
using std::vector;
using std::map;
using std::string;

namespace wd{

class WebPage{
public:
	WebPage(int,string,string,string,vector<string>&&,map<string,int>&&);
	void debug();
	friend bool operator==(const WebPage&lhs,const WebPage& rhs);
	friend bool operator<(const WebPage& lrh,const WebPage& rhs);
	int getId()const;
	const string& getTitle()const;
	const string& getLink()const;
	const string& getContent()const;
	const map<string,int>& getWordsAndFre()const;
private:
	int m_id;
	string m_title;
	string m_link;
	string m_content;
	vector<string> m_topWords;
	map<string,int> m_WordsFre;
};
}//end of namespace wd
