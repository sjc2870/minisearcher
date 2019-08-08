/*************************************************************************
 > File Name: DirScanner.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Sun 04 Aug 2019 07:40:28 AM PDT
 ************************************************************************/
#include "Config.h"
#include "DirScanner.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
using std::cout;
using std::endl;
namespace wd{

	DirScanner::DirScanner(Config& c)
	:m_config(c){}

	void DirScanner::scan(){
		map<string,string> filepath = m_config.getContent();
		string sdir = filepath["yuliao"];
		DIR* dir = opendir(sdir.c_str());
		struct dirent* d;
		while((d = readdir(dir))){
			string filename = d->d_name;
			string fullpath;
			if(filename!="."&&filename!=".."){
				fullpath = sdir+"/"+filename;
				m_xmlFilePath.push_back(fullpath);
			}
		}
	}

	vector<string>& DirScanner::getXMLFilePath(){
		return m_xmlFilePath;
	}
}//end of namespace wd
