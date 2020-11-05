#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "../cgilib.h"
#include "../_testlib.h"

class mytest: public cgi::test::unit_test {
public:
  int run() {
    return 0;
  }
};

int main(int argc, char** argv) {
  mytest unittest;
  
  return 0;
}

