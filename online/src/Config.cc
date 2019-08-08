/*************************************************************************
 > File Name: Config.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Tue 06 Aug 2019 05:49:38 PM PDT
 ************************************************************************/

#include "Config.h"
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
using std::ifstream;
namespace wd{

Config::Config(const string path)
:m_path(path){
readConfig();
}


void Config::readConfig(){
	ifstream ifs(m_path);
	if(!ifs.good()){
		cout << "error:Config::readConfig:bad file" << endl;
		return ;
	}

	string key,value;
	while(ifs.good()){
		ifs >> key >> value;
		m_config.insert(std::make_pair(key,value));
	}
}

map<string,string>& Config::getConfig(){
	return m_config;
}

void Config::debug(){
	for(auto& ele:m_config){
		cout << ele.first << ":" << ele.second << endl;
	}
}

string Config::getFileName(string name){
	auto it = m_config.find(name);
	if(it==m_config.end()){
		string nulls;
		return nulls;
	}else{
		return it->second;
	}
}
}//end of namespace wd
