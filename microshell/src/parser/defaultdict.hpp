#include <map>
#include <string>

////////////////////////////////////
// Class for storing variables
////////////////////////////////////

#ifndef DEFAULTDICT
#define DEFAULTDICT

class DefaultDict : private std::map<std::string, std::string>
{
public:

  using std::map<std::string, std::string>::clear;
  using std::map<std::string, std::string>::operator[];

  std::string at(const std::string &key)  {
    if (count(key))
      return std::map<std::string, std::string>::operator[](key);
    else
      return std::string("");
  };
};

#endif
