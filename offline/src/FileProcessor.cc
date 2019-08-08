/*************************************************************************
 > File Name: FileProcessor.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Sun 04 Aug 2019 07:45:16 PM PDT
 ************************************************************************/

#include "FileProcessor.h"
#include "tinyxml2.h"
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using std::multimap;
using std::cout;
using std::endl;
using std::regex;
using std::ifstream;
using namespace tinyxml2;
using std::istringstream;
const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";
namespace wd{
	FileProcessor::FileProcessor(vector<string>& filepath)
	:m_filePath(filepath)
	,m_jieba(DICT_PATH,
			HMM_PATH,
			USER_DICT_PATH,
			IDF_PATH,
			STOP_WORD_PATH){
		readStopWords("./dict/stop_words.utf8");
	}
	
	void FileProcessor::readStopWords(const string&path){
		ifstream ifs(path);
		string word;
		while(ifs.good()){
			ifs >> word;
			m_stopWords.insert(word);
		}
	}
	set<WebPage> FileProcessor::create(){
		set<WebPage> ret;
		int id = 1;
		for(auto &file:m_filePath){
			XMLDocument doc;
			doc.LoadFile(file.c_str());
			cout << file << endl;
			if(doc.ErrorID()){
				cout << "bad file" << endl;
			}else{
				cout << "good file" << endl;
			}
			XMLElement* rssElement = doc.FirstChildElement("rss");
			XMLElement* channelElement = nullptr;
			XMLElement* itemElement = nullptr;;

			if(rssElement){
				channelElement = rssElement->FirstChildElement("channel");
				if(channelElement){
					itemElement = channelElement->FirstChildElement("item");
				}
			}
			if(itemElement){
				cout << "no empty itemElement" << endl;
			}else{
				cout << "empty itemElement" << endl;
			}
			while(itemElement){
				cout << "id =" << id << endl;
				if(id==1){
					ret.insert(process(itemElement,id,true));
				}else{
					ret.insert(process(itemElement,id,false));
				}
				++id;
				itemElement = itemElement->NextSiblingElement("item");
			}
		}
		return ret;
	}
	WebPage FileProcessor::process(XMLElement* itemElement,int id,bool debug){
		XMLElement* titleElement = itemElement->FirstChildElement("title");
		XMLElement* linkElement = itemElement->FirstChildElement("link");
		XMLElement* contentElement = itemElement->FirstChildElement("content:encoded");
		XMLElement* summaryElement = itemElement->FirstChildElement("description");
		const char* title = nullptr;
		const char* link = nullptr;
		const char* content = nullptr;
		const char* summary = nullptr;
		if(titleElement){
			title = titleElement->GetText();
		}
		if(linkElement){
			link = linkElement->GetText();
		}
		if(contentElement){
			content = contentElement->GetText();
		}
		if(summaryElement){
			summary = summaryElement->GetText();
		}
		if(debug){
			if(title){
				cout << "title:" << title << endl;
			}else{
				cout << "no title" << endl;
			}
			if(link){
				cout << "link:" << link << endl;
			}else{
				cout << "not link" << endl;
			}
			if(content){
				cout << "content:" << content << endl;
			}else{
				cout << "no content" << endl;
			}
			if(summary){
				cout << "summary:" << summary << endl;
			}else{
				cout << "no summary" << endl;
			}
		}
		map<string,int> WordsAndFre;
		vector<string> topk;
		string summaryHandled;
		regex re("<[^>]*>");
		if(summary){
			summaryHandled = std::regex_replace(summary,re,"");
		}
		if(!content){
			if(summary){
				content = summary;
			}else{
				string nullcontent;
				return WebPage(id,title,link,nullcontent,std::move(topk),std::move(WordsAndFre));
			}
		}else if(strlen(summary)>strlen(content)){
			content = summary;
		}
		string contentHandled;
		contentHandled = std::regex_replace(content,re,"");
		contentHandled = contentHandled.size()>summaryHandled.size()?contentHandled:summaryHandled;
		if(debug){
			cout << "contentHandled:----------------" <<endl;
			cout << contentHandled << endl;
		}
		if(contentHandled.size()>0){
			vector<string> separatedContent(separateWords(contentHandled));
			if(debug){
				cout << "separateWords.size = " << separatedContent.size() << endl;
				for(auto &ele:separatedContent){
					cout << ele << " ";
				}
				cout << endl;
			}
			WordsAndFre = getFrequency(separatedContent);
			if(debug){
				cout << "wordsAndFre.size = " <<WordsAndFre.size() <<endl;
				for(auto &ele:WordsAndFre){
					cout << ele.first << "-->" << ele.second << endl;
				}
			}
			topk = getTopk(WordsAndFre);
			if(debug){
				cout << "topk.size = " <<topk.size() <<endl;
			}

		}
		string title1;
		string link1;
		if(title){
			title1 = title;
		}
		if(link){
			link1 = link;
		}
		return WebPage(id,title1,link1,contentHandled,std::move(topk),std::move(WordsAndFre));
	}

	map<string,int> FileProcessor::getFrequency(const vector<string>&content){
		map<string,int> ret;
		for(auto&word:content){
			auto it0 = m_stopWords.find(word);
			if(it0==m_stopWords.end()){
				auto it = ret.find(word);
				if(it==ret.end()){
					ret.insert(std::make_pair(word,1));
				}else{
					++it->second;
				}
			}
		}
#if 0
		for(auto&ele:ret){
			cout << ele.first << ":" << ele.second << " ";
		}
		cout << endl;
#endif
		return ret;
	}

	vector<string> FileProcessor::getTopk(map<string,int>& WordsAndFre){
		vector<string> ret;
		multimap<int,string> mid;
		for(auto&ele:WordsAndFre){
			mid.insert(std::make_pair(ele.second,ele.first));
		}
#if 0
		for(auto&ele:mid){
			cout << ele.first << ":" << ele.second << endl;
		}
#endif
		auto it = mid.rbegin();
		for(int i = 0;i<10&&it!=mid.rend();++it){
			//cout << "i = " << i << endl;
			//cout << "it->second = " << it->second << endl;
			auto it2 = m_stopWords.find(it->second);
			if(it2==m_stopWords.end()){
				//cout << it->second << "不在停用词" << endl;
				ret.push_back(it->second);
				++i;
			}else{
				//cout << it->second << "在停用词" << endl;
			}
		}
#if 0
		cout << "---------------" << endl;
		for(auto&ele:ret){
			cout << ele << " ";
		}
#endif
		return ret;
	}

	vector<string> FileProcessor::separateWords(const string& content){
		vector<string> words;
		string word;
		string result;
		m_jieba.Cut(content,words,true);	
		return words;
	}
}
