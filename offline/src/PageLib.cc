/*************************************************************************
 > File Name: PageLib.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Mon 05 Aug 2019 06:08:04 PM PDT
 ************************************************************************/

#include "PageLib.h"
#include <fstream>
#include <iostream>
#include <cmath>
using std::ofstream;
using std::endl;
using std::cout;
namespace wd{

PageLib::PageLib(set<WebPage>& pagelib,Config& config)
:m_pageLib(pagelib)
,m_config(config){
}

void PageLib::writeToFile(){
	string pagelibFileName = m_config.getName("newpagelib");
	string offsetFileName = m_config.getName("newoffsetlib");
	ofstream ofs_page(pagelibFileName.c_str());
	if(!ofs_page.good()){
		cout << "writeToFile:bad pagelibfile" << endl;
	}
	ofstream ofs_offset(offsetFileName.c_str());
	if(!ofs_offset.good()){
		cout << "writeToFile:bad offsetlibfile" << endl;
	}
	int pos1;
	int pos2;
	int id;
	for(auto &page:m_pageLib){
		id = page.getId();
		pos1 = ofs_page.tellp();
		ofs_offset << id << "\t" << pos1;

		ofs_page << "<doc>" << endl
				 << "<docid> " << id << " </docid>" << endl
				 << "<title> " << page.getTitle() << " </title>" << endl
				 << "<link> " << page.getLink() << " </link>" << endl
				 << "<content> " << page.getContent() << " </content>" << endl;

		pos2 = ofs_page.tellp();
		ofs_offset << "\t" << pos2-pos1 << endl;
	}
}
	void PageLib::createInvertIndexTable(){
		//double类型先为tf,在乘以idf得出w,在除以每篇文章的权重
		int df;
		double idf,w;
		vector<double> wn;
		int n = m_pageLib.size();
		string word;
		map<string,int> wordsAndFre;
		map<int,double> weightAll;//保存每篇文章的权重平方和，int为文章id,double为权重平方和
		vector<std::pair<string,double>> idx;//存放计算mid过程中算出的w和word
		for(auto &page:m_pageLib){
			wordsAndFre = page.getWordsAndFre();
			for(auto& ele:wordsAndFre){
				m_invertIndexTable[ele.first].push_back(std::make_pair(page.getId(),ele.second));
			}
		}
		
		for(auto&ele:m_invertIndexTable){
			df = ele.second.size();
			idf = log2(static_cast<double>(n)/(df+1));
			for(auto& ele2:ele.second){
				w = ele2.second*idf;
				ele2.second = w;
				weightAll[ele2.first] += pow(w,2);
			}
		}

		for(auto& ele:m_invertIndexTable){
			for(auto& ele2:ele.second){
				ele2.second = ele2.second/sqrt(weightAll[ele2.first]);
			}
		}
	}

	void PageLib::printInvertIndexTable(){
		string filename = m_config.getName("invertIndexlib");
		ofstream ofs(filename.c_str());
		for(auto& ele:m_invertIndexTable){
			ofs << ele.first << "   ";
			for(auto &ele2:ele.second){
				ofs << ele2.first << " " << ele2.second << "    ";
			}
			ofs << endl;
		}
	}
}//end of namespace wd
