#pragma once
#include <string>
#include <map>
using std::string;
using std::map;
namespace wd{

class Config{
public:
	Config(const string);
	map<string,string>& getConfig();
	void readConfig();
	void debug();
	string getFileName(string);
private:
string m_path;
map<string,string> m_config;
};
}//end of namespace wd
