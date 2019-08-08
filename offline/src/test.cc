/*************************************************************************
 > File Name: test.cc
 > Author: SJC
 > Mail: 1123305914@qq.com 
 > Created Time: Sun 04 Aug 2019 07:13:14 AM PDT
 ************************************************************************/

#include "Config.h"
#include "DirScanner.h"
#include "FileProcessor.h"
#include "WebPage.h"
#include "PageLib.h"
#include<iostream>
using std::cout;
using std::endl;
using namespace wd;
int main(int argc,char* argv[]){
	Config c("../config/myconf.conf");
	DirScanner ds(c);
	ds.scan();
	vector<string> xmlFilePath = ds.getXMLFilePath();
	FileProcessor fp(xmlFilePath);
	set<WebPage> libpage = fp.create();
	cout << "libpage.size = " << libpage.size() << endl;
	PageLib pl(libpage,c);
	pl.writeToFile();
	cout << "网页库也偏移库写入完毕" << endl;
	pl.createInvertIndexTable();
	cout << "倒排索引表创建完毕" << endl;
	pl.printInvertIndexTable();
	cout << "倒排索引表写入完毕" << endl;
}

