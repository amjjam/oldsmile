#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

#include <string>
#include <vector>

std::vector<std::string> stringsep(std::string s, char c=' ');
std::string stringcompress(std::string s, char c=' ');
std::string stringtrim(std::string s, char c=' ');

#endif
