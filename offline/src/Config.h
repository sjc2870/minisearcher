#pragma once
#include <string>
#include <map>
using std::map;
using std::string;

namespace wd{

class Config{
public:
	Config(const string&);
	void readConfig();
	map<string,string>& getContent();
	void debug();
	string getName(string);
private:
string m_filePath;
map<string,string> m_content;
};
}//end of namespace wd
