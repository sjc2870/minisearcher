/*************************************************************************
 > File Name: WordQuery.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Tue 06 Aug 2019 08:29:09 PM PDT
 ************************************************************************/

#include "WordQuery.h"
#include <json/json.h>
#include <cmath>
#include <functional>
using std::ifstream;
namespace wd{
	WordQuery::WordQuery(Config& config,Jieba& jieba)
	:m_config(config)
	,m_jieba(jieba){
		readStopWords();
		loadFile();
#if 1
		cout << "m_invertIndexTable.size() = " << m_invertIndexTable.size() << endl;
		cout << "m_pageLib.size() = " << m_pageLib.size() << endl;
		cout << "m_offsetLib.size() = " << m_offsetLib.size() << endl;
#endif
	}
	
	void WordQuery::readStopWords(){
//---------------------读入停用词
		ifstream ifs("./dict/stop_words.utf8");
		if(!ifs.is_open()){
			cout << "readStopWords:ifs doesn't associate a file" << endl;
			return;
		}
		string word;
		while(ifs.good()){
			ifs >> word;
			m_stopWords.insert(word);
		}
	}

	void WordQuery::loadFile(){
		//加载网页库和偏移库，并创建m_pagelib
//-----------------读入文件路径
		string pageLibFileName = m_config.getFileName("newpagelib");
		string offsetLibFileName = m_config.getFileName("newoffsetlib");
		if(pageLibFileName.size()==0||offsetLibFileName.size()==0){
			cout << "error:loadFile:badfile" << endl;
			return;
		}
//-----------------读入偏移库和网页库
		ifstream ifs_offset(offsetLibFileName.c_str());
		ifstream ifs_pagelib(pageLibFileName.c_str());
		int id,offset,len;
		if(!ifs_offset.is_open()){
			cout << "error:loadfile:ifs_offset doesn't associate a file" << endl;
			return;
		}
		if(!ifs_pagelib.is_open()){
			cout << "error:loadfile:ifs_pagelib doesn't associate a file" << endl;
			return ;
		}
		while(ifs_offset.good()){
			//读入偏移库
			ifs_offset >> id >> offset >> len;
			m_offsetLib.insert(std::make_pair(id,std::make_pair(offset,len)));
			//读入网页库
			string doc;
			doc.resize(len+1,' ');
			ifs_pagelib.seekg(offset);
			ifs_pagelib.read(&*doc.begin(),len);

			m_pageLib.push_back(WebPage(doc,m_jieba,m_stopWords));
		}
		ifs_offset.close();
		ifs_pagelib.close();
//-------------------------读入倒排索引表
		string filename = m_config.getFileName("invertIndexlib");
		ifstream ifs(filename.c_str());
		if(!ifs.is_open()){
			cout << "loadfile: ifs doesn't associate a file" << endl;
			return;
		}else{
			cout << "filename = " << filename << endl;
		}
		string word;
		string line;
		double weight;
		while(ifs.good()){
			std::getline(ifs,line);
			istringstream iss(line);
			iss >> word;
			while(iss.good()){
				iss >> id >> weight;
				m_invertIndexTable[word].push_back(std::make_pair(id,weight));
			}
		}
	}

	string WordQuery::query(const string& msg){
		//对消息进行分词，看哪篇文章包括这些词，把都包括这些词的文章做一个排序
		string ret;
		vector<string> words;
		m_jieba.Cut(msg,words,true);
		int n;
		multiset<int> se;
		set<int> retID;
		//se中保存分别包含这些词的文章ID,retID中保存结果文章的id
		for(auto& word:words){
			auto it = m_invertIndexTable.find(word);
			if(it!=m_invertIndexTable.end()){
				for(auto& ele:it->second){
					se.insert(ele.first);
				}
			}
		}
#if 0
		cout << endl;
		cout << "se.size = " << se.size() << endl;
#endif
		n = words.size();
		//对se中元素进行计数，如果有数量等于words.size的，加入结果文章
		int cnt;
		for(auto& num:se){
			cnt = se.count(num);
			if(cnt>=n){
				retID.insert(num);
			}
		}
		if (retID.size()==0){
			//没找到符合要求的文章
			return notFound();
		}
#if 1
		cout << "包含" << msg << "的文章id有：" << endl;
		for(auto& num:retID){
			cout << num << " ";
		}
		cout << endl;
#endif
		//sortedID中保存排序之后文章的ID
		vector<int> sortedID = sortPages(retID,words);
#if 1
		cout << "文章排序后的顺序为:" << endl;
		for(auto &num:sortedID){
			cout << num << " ";
		}
		cout << endl;
#endif
		ret = toJson(sortedID,words);
		return ret;
	}

	string WordQuery::toJson(vector<int>& sortedID, vector<string>& msg){
		Json::Value ret;
		Json::Value arr;
		int cnt = 0;
		for(auto& id:sortedID){
			string title = m_pageLib[id-1].getTitle();
			string link = m_pageLib[id-1].getLink();
			string summary = m_pageLib[id-1].getSummary(msg);

			Json::Value elem;
			elem["title"] = title;
			elem["summary"] = summary;
			elem["url"] = link;
			arr.append(elem);
			if(++cnt==100){
				//限制文章的数量，不能超过100篇
				break;
			}
		}
		ret["files"] = arr;
		Json::StyledWriter writer;
		return writer.write(ret);
	}
	string WordQuery::notFound(){
		Json::Value root;
		Json::Value arr;
		
		Json::Value ret;
		Json::FastWriter writer;
		root["title"] = "404,not found";
		root["summary"] = "没找到...可能是点击不够用力呢...";
		root["rul"] = "";
		arr.append(root);
		ret["file"] = arr;

		return writer.write(ret);
	}
	vector<int> WordQuery::sortPages(set<int>& pagesID,vector<string>& words){
//-------------------将消息看成一篇文档，计算这个文档中每个单词的权重值
		vector<double> msg_w;//消息的权重值
		msg_w.resize(words.size(),1);
		int df;
		string word;
		double weightall = 0;
		double idf;
		double weight;
		int pagesNum = m_pageLib.size();
		for(int i = 0;i<msg_w.size();++i){
			word = words[i];
			df = m_invertIndexTable[word].size();
			idf = log2(pagesNum/(df+1));
			weight = idf;
			msg_w[i] = weight;
			weightall += pow(weight,2);
		}
		weightall = sqrt(weightall);
		for(auto& num:msg_w){
			num = num/weightall;
		}
//---------------------消息单词的权重值计算完毕,下面的mvd存放每个单词在每篇文章的权重
		map<double,int,std::greater<double>> sortedID;//存放cos和文章id
		map<int,vector<double>> mvd;//文章id.每个单词在文章中的权重值
		int n = pagesID.size();
		int id;
		for(int i = 0;i<n;++i){
			//对欲排序的每篇文章进行操作
			for(int j = 0;j<words.size();++j){
				//对每个words中的单词进行操作
				auto it2 = m_invertIndexTable.find(words[j]);
				vector<pair<int,double>> weightOfWord = it2->second;
				//存放某个单词在每篇文章的权重，id--weight
				for(auto&ele:weightOfWord){
					id = ele.first;
					if(pagesID.count(id)>0){
						//如果这篇文章是结果文章之一,那么将这个单词在这篇文章的权重值加入mvd
						mvd[id].push_back(ele.second);
					}
				}
			}
		}

		double cos;
		double XY;
		double absX,absY;
		for(auto it = pagesID.begin();it!=pagesID.end();++it){
			id = *it;
			XY = 0;
			absX = 0;
			absY = 0;
			cos = 0;
			for(int i = 0;i<msg_w.size();++i){
				XY += msg_w[i]*mvd[id][i];
				absX += pow(msg_w[i],2);
				absY += pow(mvd[id][i],2);
			}
			cos = XY/(absX*absY);
			sortedID.insert(std::make_pair(cos,id));
		}

		vector<int> ret;
		for(auto& ele:sortedID){
			ret.push_back(ele.second);
		}
		return ret;
	}
}//end of namespace wd
