#include <iostream>

#include "../cgilib.h"

int main(int argc, char** argv) {
  cgi::headers.add("Content-Type", "text/plain");

  cgi::out << "It works!" << std::endl;

  cgi::send_response();
  
  return 0;
}

