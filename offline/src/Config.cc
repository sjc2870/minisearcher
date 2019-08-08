/*************************************************************************
  > File Name: Config.cc
  > Author: SJC
  > Mail: 1123305914@qq.com 
  > Created Time: Sun 04 Aug 2019 06:52:19 AM PDT
 ************************************************************************/
#include "Config.h"
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
namespace wd{

Config::Config(const string&filepath)
	:m_filePath(filepath){
		readConfig();
	}

void Config::readConfig(){
	ifstream ifs(m_filePath.c_str());
	string line;
	while(ifs.good()){
		std::getline(ifs,line);
		istringstream iss(line);
		while(iss.good() && line.size() >0){
			string key,value;
			iss >> key >> value;
			m_content.insert(std::make_pair(key,value));
		}
	}
}

void Config::debug(){
	for(auto &ele:m_content){
		cout << ele.first << ":" << ele.second << endl;
	}
	cout << "m_content.size = " << m_content.size() << endl;
}
map<string,string>& Config::getContent(){
	return m_content;
}

string Config::getName(string name){
	return m_content[name];
}
}//end of namespace wd
