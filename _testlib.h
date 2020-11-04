#ifndef CGILIB_TESTING_LIBRARY_H_
#define CGILIB_TESTING_LIBRARY_H_

#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>

extern char* environ;

namespace cgi {
namespace test {

class __startup__ {
public:
  __startup__() {
    // SEE #15 TO UNDERSTAND THIS
    // Step 1: Clear environment
    for(char* env = environ; *env; ++env) {
      std::string varline = *env;
      std::string varname = varline.substr(0, varline.find('='));
      unsetenv(varname.c_str());
    }

    // Step 2: Initialize STDIN
    std::istringstream initial("");
    std::cin.rdbuf(initial.rdbuf());

    // Step 3: Initialize all meta-variables [rfc3875]
    setenv("AUTH_TYPE",          ""              );  // Default environment has no authorization
    setenv("CONTENT_LENGTH",     ""              );  // Default environment has no request body
    setenv("CONTENT_TYPE",       ""              );  // Default environment has no request body
    setenv("GATEWAY_INTERFACE",  "CGI/1.1"       );  // Default environment emulates CGI version 1.1
    setenv("PATH_INFO",          ""              );  // Default environment has no crazy URI stuff
    setenv("PATH_TRANSLATED",    ""              );  // Default environment has no crazy URI stuff
    setenv("QUERY_STRING",       "foo=bar"       );  // Default environment is a GET request
    setenv("REMOTE_ADDR",        "192.168.1.2"   );  // Default environment has no useful IP examples
    setenv("REMOTE_HOST",        ""              );  // Default environment is localhost
    setenv("REQUEST_METHOD",     "GET"           );  // Default environment is a GET request
    setenv("SCRIPT_NAME",        ""              );  // Default environment has no crazy URI stuff
    setenv("SERVER_NAME",        "127.0.0.1"     );  // Default environment is localhost
    setenv("SERVER_PORT",        "8080"          );  // Default environment runs on port 8080
    setenv("SERVER_PROTOCOL",    "HTTP/1.1"      );  // Default environment uses HTTP version 1.1
    setenv("SERVER_SOFTWARE",    "SimpleHTTP/0.6");  // Default environment uses SimpleHTTP version 0.6
    
    // Step 4: Model HTTP headers
    setenv("HTTP_HOST",          "localhost"     );
    setenv("HTTP_USER_AGENT",    "Hello, world!" );
    setenv("HTTP_CACHE_CONTROL", "max-age=0"     );
    setenv("HTTP_ACCEPT",        "*/*"           );
  }
};

__startup__ __do_some_startup_code___;

} // namespace test
} // namespace cgi

#endif  // CGILIB_TESTING_LIBRARY_H_

