#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "../cgilib.h"
#include "../_testlib.h"

class mytest: public cgi::test::unit_test {
public:
  mytest(std::string s) : unit_test(s) {
    create_header("Accept", "text/plain");
  }

  int run() {
    cgi::headers.add("Content-Type", "text/plain");
    cgi::out << "Hello, there!" << std::endl;

    cgi::send_response();
    return 0;
  }
};

int main(int argc, char** argv) {
  mytest unittest("mytest");

  cgi::test::execute_test(unittest);
  
  return 0;
}

