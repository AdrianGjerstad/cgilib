#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "../cgilib.h"
#include "../_testlib.h"

class mytest: public cgi::test::unit_test {

};

int main(int argc, char** argv) {
  mytest unittest;
  
  return 0;
}

