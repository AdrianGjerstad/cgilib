#ifndef CGILIB_TESTING_LIBRARY_H_
#define CGILIB_TESTING_LIBRARY_H_

#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

#include <ctime>  // In conjunction with <cstdlib>, we have pseudo-random numbers

extern char** environ;

namespace cgi {
namespace test {

class __startup__ {
public:
  __startup__() {
    // SEE #15 TO UNDERSTAND THIS
    for(char** env = environ; *env; ++env) {
      std::string varline = *env;
      std::string varname = varline.substr(0, varline.find('='));
      unsetenv(varname.c_str());
    }
  }
};

__startup__ __do_some_startup_code___;

// Create a class that inherits from this class to write a test.
class unit_test {
public:
  unit_test() {
    srand(time(NULL));  // Seed number gen
  }
private:
  std::vector<std::pair<std::string, std::vector<std::string>>> headers;

  void create_header(std::string name, std::string field) {
    std::vector<std::string> emptyvec{};
    int entry_index = -1;

    for(int i = 0; i < headers.size(); ++i) {
      if(headers[i].first == name) {
        entry_index = i;
        break;
      }
    }

    if(entry_index == -1) {
      headers.push_back(std::make_pair(name, emptyvec));
      entry_index = headers.size()-1;
    }

    headers[entry_index].second.push_back(field);
  }

  std::string read_header(std::string name) {
    int entry_index = -1;

    for(int i = 0; i < headers.size(); ++i) {
      if(headers[i].first == name) {
        entry_index = i;
        break;
      }
    }

    if(entry_index == -1) {
      return "";
    }
    
    std::string result;
    std::string sep = ", ";
    
    // To reduce the use of memory reallocation, we'll determine result size beforehand
    int alloc_size = headers[entry_index].second.size()*2 - 2;
    for(int i = 0; i < headers[entry_index].second.size(); ++i) {
      alloc_size += headers[entry_index].second[i].size();
    }

    result.reserve(alloc_size);
    
    for(int i = 0; i < headers[entry_index].second.size(); ++i) {
      if(name == "Cookie") {  // Cookie has a semicolon (;) separator, not comma (,).
        // Still good practice to test both ways, since it could be given either way.
        if(rand() > RAND_MAX/2) {
          sep = "; ";
        }
      }

      if(i != 0) {
        result += sep;
      }

      result += headers[entry_index].second[i];
    }

    return result;
  }
};

} // namespace test
} // namespace cgi

#endif  // CGILIB_TESTING_LIBRARY_H_

