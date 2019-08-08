/*************************************************************************
 > File Name: WebPage.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Sun 04 Aug 2019 11:29:20 PM PDT
 ************************************************************************/

#include "WebPage.h"
#include <iostream>
using std::cout;
using std::endl;
namespace wd{
	WebPage::WebPage(int id,string title,string link,string content,vector<string>&&topk,map<string,int>&&WordsFre)
	:m_id(id)
	,m_title(title)
	,m_link(link)
	,m_content(content)
	,m_topWords(topk)
	,m_WordsFre(WordsFre){}

	void WebPage::debug(){
		cout << "id = " << m_id << endl;
		cout << "title = " << m_title << endl;
		cout << "link = " << m_link << endl;
		cout << "content = " << m_content << endl;
		cout << "topk words:" << endl;
		for(auto &ele:m_topWords){
			cout << ele << " ";
		}
		cout << endl;
	}

	int WebPage::getId()const{
		return m_id;
	}

	const string& WebPage::getTitle()const{
		return m_title;
	}

	const string& WebPage::getLink()const{
		return m_link;
	}

	const string& WebPage::getContent()const{
		return m_content;
	}

	const map<string,int>& WebPage::getWordsAndFre()const {
		return m_WordsFre;
	}
#if 1
	bool operator==(const WebPage& lhs,const WebPage &rhs){
		int n = 0;
		for(auto &word0:lhs.m_topWords){
			for(auto& word1:rhs.m_topWords){
				if(word0==word1){
					++n;
				}
			}
		}
		if(n>=6){
			return true;
		}
		return false;
	}

	bool operator<(const WebPage& lhs,const WebPage& rhs){
		return lhs.m_id<rhs.m_id;
	}
#endif
}//end of namespace wd
