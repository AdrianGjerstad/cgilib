#ifndef CGILIB_TESTING_LIBRARY_H_
#define CGILIB_TESTING_LIBRARY_H_

#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <sys/time.h>

#include <ctime>  // In conjunction with <cstdlib>, we have pseudo-random numbers

#include "cgilib.h"

extern char** environ;

namespace cgi {
namespace test {

long __timeval_to_seconds(timeval tv) {
  return (tv.tv_sec * 1000000) + tv.tv_usec;
}

// Create a class that inherits from this class to write a test.
class unit_test {
public:
  unit_test(std::string name) {
    name_ = name;
    srand(time(NULL));  // Seed number gen
    clear_environment();  // Start with a fresh slate
  }

  void build_environment() {
    // Build headers
    for(int i = 0; i < headers.size(); ++i) {
      std::string name = headers[i].first;
      for(int j = 0; j < name.size(); ++j) {
        if(name[j] == '-') {
          name[j] == '_';
        } else {
          name[j] = ::cgi::__string_asciitoupper__(name[j]);
        }
      }

      setenv((std::string("HTTP_") + name).c_str(), read_header(headers[i].first).c_str(), 1);
    }
  }

  void print_request_headers() {
    for(int i = 0; i < headers.size(); ++i) {
      std::string name = headers[i].first;
      for(int j = 0; j < headers[i].second.size(); ++j) {
        std::cout << "\033[48;33m" << name << ": \033[48;31m" << headers[i].second[j] << "\033[0m" << std::endl;
      }
    }
  }

  std::string get_name() {
    return name_;
  }

  void clear_environment() {
    // SEE #15 TO UNDERSTAND THIS
    for(char** env = environ; *env; ++env) {
      std::string varline = *env;
      std::string varname = varline.substr(0, varline.find('='));
      unsetenv(varname.c_str());
    }
  }

  void clear_headers() {
    headers = {};
  }

  virtual int run() = 0;
  virtual void initialize() = 0;
private:
  std::string name_;
  std::vector<std::pair<std::string, std::vector<std::string>>> headers;
protected:
  bool gen_chance(int numerator, int denominator) {
    return rand() <= (((RAND_MAX)/denominator) * numerator);
  }

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
  
  // Begin setters for metavariables [rfc3875 section 4.1]

  void set_auth_type(std::string type) {
    setenv("AUTH_TYPE", type.c_str(), 1);
  }

  void set_content_length(int length) {
    setenv("CONTENT_LENGTH", std::to_string(length).c_str(), 1);
  }

  void set_content_type(std::string type) {
    setenv("CONTENT_TYPE", type.c_str(), 1);
  }

  void set_cgi_version(int major, int minor) {
    setenv("GATEWAY_INTERFACE", (std::string("CGI/") + std::to_string(major) + "." + std::to_string(minor)).c_str(), 1);
  }

  void set_path_info(std::string info) {
    setenv("PATH_INFO", info.c_str(), 1);
  }

  void set_translated_path(std::string path) {
    setenv("PATH_TRANSLATED", path.c_str(), 1);
  }

  void set_query_string(std::string query) {
    setenv("QUERY_STRING", query.c_str(), 1);
  }

  void set_client_address(std::string remote) {
    setenv("REMOTE_ADDR", remote.c_str(), 1);
  }

  void set_client_name(std::string name) {
    setenv("REMOTE_HOST", name.c_str(), 1);
  }

  void set_client_identity(std::string identity) {
    setenv("REMOTE_IDENT", identity.c_str(), 1);
  }

  void set_user_string(std::string user) {
    setenv("REMOTE_USER", user.c_str(), 1);
  }

  void set_request_method(std::string method) {
    setenv("REQUEST_METHOD", method.c_str(), 1);
  }

  void set_script_name(std::string name) {
    setenv("SCRIPT_NAME", name.c_str(), 1);
  }

  void set_address(std::string address) {
    setenv("SERVER_NAME", address.c_str(), 1);
  }

  void set_port(int port) {
    setenv("SERVER_PORT", std::to_string(port).c_str(), 1);
  }

  void use_http() {
    set_port(80);
  }

  void use_https() {
    set_port(443);
  }

  void set_http_version(int major, int minor) {
    setenv("SERVER_PROTOCOL", (std::string("HTTP/") + std::to_string(major) + "." + std::to_string(minor)).c_str(), 1);
  }

  void set_server_software(std::string software) {
    setenv("SERVER_SOFTWARE", software.c_str(), 1);
  }

  void set_request_body(std::string body) {
    std::istringstream b(body);
    std::cin.rdbuf(b.rdbuf());
  }
};

void execute_test(unit_test& test) {
  int trial_count = 25;
  int max_avg_time = 20000;  // measured in microseconds
  
  long usec_sum = 0;

  for(int i = 0; i < trial_count; ++i) {
    std::cout << "\033[48;34;1m" << "Running test: " << test.get_name() << " (" << i+1 << " of " << trial_count << ")" << "\033[0m" << std::endl;
    
    ::cgi::headers.clear();
    ::cgi::out.str("");
    test.clear_environment();
    test.clear_headers();
    test.initialize();
    test.build_environment();
    ::cgi::request_headers.regenerate_list();

    std::cout << "\033[48;35;1m" << "Request headers given to script: " << "\033[0m" << std::endl;
    test.print_request_headers();

    std::cout << std::endl << "\033[48;34;1m" << "Script Output:" << "\033[0m" << std::endl;

    timeval tv;
    gettimeofday(&tv, 0);

    long t_0 = __timeval_to_seconds(tv);
    int res = test.run();
    gettimeofday(&tv, 0);
    long t_f = __timeval_to_seconds(tv);

    usec_sum += t_f - t_0;

    if(res != 0) {
      std::cout << "\033[48;31;1m" << "Error occured in test (via return) " << test.get_name() << "\033[0m" << std::endl;
    }
  }

  if(usec_sum >= trial_count * max_avg_time) {
    std::cout << "\033[48;33;1m" << "WARNING: Average execution time was longer than " << max_avg_time/1000 << "ms (" << usec_sum/1000 << "ms)" << "\033[0m" << std::endl;
  }
}

} // namespace test
} // namespace cgi

#endif  // CGILIB_TESTING_LIBRARY_H_

