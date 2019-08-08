#pragma once
#include <vector>
#include <string>
using std::string;
using std::vector;

namespace wd{
class Config;
class DirScanner{
public:
	DirScanner(Config&);
	void scan();
	vector<string>& getXMLFilePath();
private:
	vector<string> m_xmlFilePath;
	Config &m_config;
};
}//end of namespace wd
