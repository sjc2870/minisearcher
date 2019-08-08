/*************************************************************************
 > File Name: WebPage.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Tue 06 Aug 2019 07:12:59 PM PDT
 ************************************************************************/
#include "WebPage.h"
#include<iostream>
using std::cout;
using std::endl;

namespace wd{
	WebPage::WebPage(const string& doc,Jieba& jieba,set<string>& stopWords){
		process(doc,jieba,stopWords);
	}
	
	void WebPage::process(const string&doc,Jieba& jieba,set<string>& stopWords){
		string idHead("<docid>");
		string idTail("</docid>");
		string titleHead("<title>");
		string titleTail("</title>");
		string linkHead("<link>");
		string linkTail("</link>");
		string contentHead("<content>");
		string contentTail("</content>");
//-------------提取文章id
		int headPos = doc.find(idHead)+idHead.size();
		int tailPos = doc.find(idTail);
		string id = doc.substr(headPos,tailPos-headPos);
		m_id = atoi(id.c_str());
//-------------提取文章title
		headPos = doc.find(titleHead)+titleHead.size();
		tailPos = doc.find(titleTail);
		m_title = doc.substr(headPos,tailPos-headPos);
//-------------提取文章link
		headPos = doc.find(linkHead)+linkHead.size();
		tailPos = doc.find(linkTail);
		m_link = doc.substr(headPos,tailPos-headPos);
//-------------提取文章content
		headPos = doc.find(contentHead)+contentHead.size();
		tailPos = doc.find(contentTail);
		m_content = doc.substr(headPos,tailPos-headPos);
//-------------对文章内容进行分词
		vector<string>words = separateWords(m_content,jieba);
//-------------创建词频
		for(auto& word:words){
			auto it = stopWords.find(word);
			if(it==stopWords.end()){
				++m_wordsAndFre[word];
			}
		}
//-------------创建topk
		auto it = m_wordsAndFre.rbegin();
		for(int i = 0;it!=m_wordsAndFre.rend()&&i<10;++it,++i){
			m_topWords.push_back(it->first);
		}
	}

	vector<string> WebPage::separateWords(const string& content,Jieba& jieba){
		vector<string> words;
		jieba.Cut(content,words,true);
		return words;
	}
	
	string WebPage::getTitle()const{
		return m_title;
	}

	string WebPage::getLink()const{
		return m_link;
	}

	string WebPage::getSummary(vector<string>& msg)const {
		string summary;
		string line;
		istringstream iss(m_content);
		while(iss.good()){
			iss >> line;
			for(auto& word:msg){
				if(line.find(word)!=string::npos){
					summary += line;
					break;
				}
			}
			if(summary.size() > 100){
				summary = summary.substr(0,200);
				break;
			}
		}
		return summary;
	}
	bool operator==(const WebPage& lhs,const WebPage& rhs){
		return lhs.m_id==rhs.m_id;
	}

	bool operator<(const WebPage& lhs,const WebPage& rhs){
		return lhs.m_id<rhs.m_id;
	}
}//end of namespace wd
