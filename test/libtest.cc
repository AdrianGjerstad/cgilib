#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <utility>

#include "../cgilib.h"
#include "../_testlib.h"

int x = 0;

class mytest: public cgi::test::unit_test {
public:
  mytest(std::string s) : unit_test(s) {};

  void initialize() {
    ++x;
    create_header("Accept", "text/plain");
    create_header("Test-Header", std::to_string(x));
  }

  int run() {
    cgi::headers.add("Content-Type", "text/plain");
    std::vector<std::pair<std::string, std::string>> list = cgi::request_headers.list();

    for(int i = 0; i < list.size(); ++i) {
      cgi::out << list[i].first << ": " << list[i].second << std::endl;
    }

    cgi::out << std::endl << "done." << std::endl;

    cgi::send_response();
    return 0;
  }
};

int main(int argc, char** argv) {
  mytest unittest("mytest");

  cgi::test::execute_test(unittest);
  
  return 0;
}

