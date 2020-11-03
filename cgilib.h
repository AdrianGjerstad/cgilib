// cgilib, a header-only Common Gateway Interface library to do everything

#ifndef CGILIB_INCLUDE_CGILIB_H_
#define CGILIB_INCLUDE_CGILIB_H_

#include <cstdlib>
#include <iostream>
#include <string>
#include <stropts.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <fstream>

// Get access to a list of every environment variable
// (Useful for listing request headers)
extern char** environ;

namespace cgi {

std::string __getenv__(const char* name) {
  char* env = std::getenv(name);
  if(env!=NULL) {
    return (std::string)env;
  }

  return "";
}

class __vars__ {
public:
  __vars__() {
    // Common Gateway Interface RFC 3875 sect 4.1.1 states:
    // The AUTH_TYPE variable identifies any mechanism used by the server to
    // authenticate the user.  It contains a case-insensitive value defined
    // by the client protocol or server implementation.
    AUTH_TYPE = __getenv__("AUTH_TYPE");
    
    // Common Gateway Interface RFC 3875 sect 4.1.2 states:
    // The CONTENT_LENGTH variable contains the size of the message-body
    // attached to the request, if any, in decimal number of octets.  If no
    // data is attached, then NULL (or unset).
    CONTENT_LENGTH = __getenv__("CONTENT_LENGTH");
    
    // Common Gateway Interface RFC 3875 sect 4.1.3 states:
    // If the request includes a message-body, the CONTENT_TYPE variable is
    // set to the Internet Media Type [6] of the message-body.
    CONTENT_TYPE = __getenv__("CONTENT_TYPE");
    
    // Common Gateway Interface RFC 3875 sect 4.1.4 states:
    // The GATEWAY_INTERFACE variable MUST be set to the dialect of CGI
    // being used by the server to communicate with the script.
    GATEWAY_INTERFACE = __getenv__("GATEWAY_INTERFACE");
    
    // Common Gateway Interface RFC 3875 sect 4.1.5 states:
    // The PATH_INFO variable specifies a path to be interpreted by the CGI
    // script.  It identifies the resource or sub-resource to be returned by
    // the CGI script, and is derived from the portion of the URI path
    // hierarchy following the part that identifies the script itself.
    // Unlike a URI path, the PATH_INFO is not URL-encoded, and cannot
    // contain path-segment parameters.  A PATH_INFO of "/" represents a
    // single void path segment.
    PATH_INFO = __getenv__("PATH_INFO");
    
    // Common Gateway Interface RFC 3875 sect 4.1.6 states:
    // The PATH_TRANSLATED variable is derived by taking the PATH_INFO
    // value, parsing it as a local URI in its own right, and performing any
    // virtual-to-physical translation appropriate to map it onto the
    // server's document repository structure.  The set of characters
    // permitted in the result is system-defined.
    PATH_TRANSLATED = __getenv__("PATH_TRANSLATED");
    
    // Common Gateway Interface RFC 3875 sect 4.1.7 states:
    // The QUERY_STRING variable contains a URL-encoded search or parameter
    // string; it provides information to the CGI script to affect or refine
    // the document to be returned by the script.
    //
    // The URL syntax for a search string is described in section 3 of RFC
    // 2396 [2].  The QUERY_STRING value is case-sensitive.
    QUERY_STRING = __getenv__("QUERY_STRING");
    
    // Common Gateway Interface RFC 3875 sect 4.1.8 states:
    // The REMOTE_ADDR variable MUST be set to the network address of the
    // client sending the request to the server.
    REMOTE_ADDR = __getenv__("REMOTE_ADDR");
    
    // Common Gateway Interface RFC 3875 sect 4.1.9 states:
    // The REMOTE_HOST variable contains the fully qualified domain name of
    // the client sending the request to the server, if available, otherwise
    // NULL.  Fully qualified domain names take the form as described in
    // section 3.5 of RFC 1034 [17] and section 2.1 of RFC 1123 [12].
    // Domain names are not case sensitive.
    REMOTE_HOST = __getenv__("REMOTE_HOST");
    
    // Common Gateway Interface RFC 3875 sect 4.1.10 states:
    // The REMOTE_IDENT variable MAY be used to provide identity information
    // reported about the connection by an RFC 1413 [20] request to the
    // remote agent, if available.  The server may choose not to support
    // this feature, or not to request the data for efficiency reasons, or
    // not to return available identity data.
    REMOTE_IDENT = __getenv__("REMOTE_IDENT");
    
    // Common Gateway Interface RFC 3875 sect 4.1.11 states:
    // The REMOTE_USER variable provides a user identification string
    // supplied by client as part of user authentication.
    REMOTE_USER = __getenv__("REMOTE_USER");
    
    // Common Gateway Interface RFC 3875 sect 4.1.12 states:
    // The REQUEST_METHOD meta-variable MUST be set to the method which
    // should be used by the script to process the request, as described in
    // section 4.3.
    REQUEST_METHOD = __getenv__("REQUEST_METHOD");
    
    // Common Gateway Interface RFC 3875 sect 4.1.13 states:
    // The SCRIPT_NAME variable MUST be set to a URI path (not URL-encoded)
    // which could identify the CGI script (rather than the script's
    // output).  The syntax is the same as for PATH_INFO (section 4.1.5)
    SCRIPT_NAME = __getenv__("SCRIPT_NAME");
    
    // Common Gateway Interface RFC 3875 sect 4.1.14 states:
    // The SERVER_NAME variable MUST be set to the name of the server host
    // to which the client request is directed.  It is a case-insensitive
    // hostname or network address.  It forms the host part of the
    // Script-URI.
    SERVER_NAME = __getenv__("SERVER_NAME");
    
    // Common Gateway Interface RFC 3875 sect 4.1.15 states:
    // The SERVER_PORT variable MUST be set to the TCP/IP port number on
    // which this request is received from the client.  This value is used
    // in the port part of the Script-URI.
    SERVER_PORT = __getenv__("SERVER_PORT");
    
    // Common Gateway Interface RFC 3875 sect 4.1.16 states:
    // The SERVER_PROTOCOL variable MUST be set to the name and version of
    // the application protocol used for this CGI request.  This MAY differ
    // from the protocol version used by the server in its communication
    // with the client.
    SERVER_PROTOCOL = __getenv__("SERVER_PROTOCOL");
    
    // Common Gateway Interface RFC 3875 sect 4.1.17 states:
    // The SERVER_SOFTWARE meta-variable MUST be set to the name and version
    // of the information server software making the CGI request (and
    // running the gateway).  It SHOULD be the same as the server
    // description reported to the client, if any.
    SERVER_SOFTWARE = __getenv__("SERVER_SOFTWARE");
    
    // Common Gateway Interface RFC 3875 sect 4.2 states:
    // Request data is accessed by the script in a system-defined method;
    // unless defined otherwise, this will be by reading the 'standard
    // input' file descriptor or file handle.
    int n;
    if(CONTENT_LENGTH != "") {
      request_body.reserve(std::stoi(CONTENT_LENGTH));
      char* body = new char[std::stoi(CONTENT_LENGTH)];
      std::cin.read(body, std::stoi(CONTENT_LENGTH));
      request_body = body;
      delete body;
    }
  }

  std::string AUTH_TYPE;
  std::string CONTENT_LENGTH;
  std::string CONTENT_TYPE;
  std::string GATEWAY_INTERFACE;
  std::string PATH_INFO;
  std::string PATH_TRANSLATED;
  std::string QUERY_STRING;
  std::string REMOTE_ADDR;
  std::string REMOTE_HOST;
  std::string REMOTE_IDENT;
  std::string REMOTE_USER;
  std::string REQUEST_METHOD;
  std::string SCRIPT_NAME;
  std::string SERVER_NAME;
  std::string SERVER_PORT;
  std::string SERVER_PROTOCOL;
  std::string SERVER_SOFTWARE;
  std::string request_body;
};

__vars__ vars;

class __headers__ {
public:
  __headers__() {}
  
  int add(std::string name, std::string value) {
    headers.push_back(std::make_pair(name, value));
    return headers.size()-1;
  }
  
  void remove(int idx) {
    headers.erase(headers.begin()+idx);
  }

  std::string format() {
    std::string res;
    for(int i = 0; i < headers.size(); ++i) {
      res += headers[i].first + ": " + headers[i].second + "\n";
    }

    return res + "\n";
  }
private:
  std::vector<std::pair<std::string, std::string>> headers;
};

__headers__ headers;

int __status_code_idx__ = -1;
void set_status_code(int status_code) {
  if(__status_code_idx__ != -1) {
    headers.remove(__status_code_idx__);
  }

  __status_code_idx__ = headers.add("Status", std::to_string(status_code));
}

// When writing scripts, output to this stringstream to send content
// without interfering with header data
//
// E.g.:
// cgi::out << "Hello, world!" << std::endl;
std::ostringstream out;

void send_response() {
  std::cout << headers.format();
  std::cout << out.str() << std::endl;
}

void redirect_with_code(int code, std::string uri) {
  std::cout << "Status: " << code << std::endl;
  std::cout << "Location: " << uri << std::endl << std::endl;

  std::exit(0);
}

void redirect(std::string uri) {
  redirect_with_code(302, uri);
}

void redirect_from_login(std::string uri) {
  redirect_with_code(303, uri);
}

void redirect_permanent(std::string uri) {
  redirect_with_code(301, uri);
}

char __string_asciitolower__(char in) {
  if (in <= 'Z' && in >= 'A')
    return in - ('Z' - 'z');
  return in;
}

char __string_asciitoupper__(char in) {
  if (in <= 'z' && in >= 'a')
    return in + ('Z' - 'z');
  return in;
}

class __request_headers__ {
public:
  __request_headers__() {
    list_generated = false;
  }

  std::string get(std::string name) {
    std::string tmp = name;

    for(int i = 0; i < tmp.size(); ++i) {
      if(tmp[i] == '-') {
        tmp[i] = '_';
      }
    }

    std::transform(tmp.begin(), tmp.end(), tmp.begin(), __string_asciitoupper__);
    
    tmp = std::string("HTTP_") + tmp;

    return __getenv__(tmp.c_str());
  }

  std::vector<std::pair<std::string, std::string>> list() {
    if(!list_generated) {
      generate_list();
    }

    return headers;
  }
private:
  void generate_list() {
    for(char** envlist = environ; *envlist; ++envlist) {
      std::string entry = *envlist;
      // Returns something like HTTP_USER_AGENT=Mozilla/1.0
      // Desired: std::pair("User-Agent", "Mozilla/1.0")
      
      if(entry.substr(0, 5) == "HTTP_") {
        std::string value = entry.substr(entry.find('=')+1);
        // CGI Scripts, while not given any hazardous content, still recieve
        // other metadata, so it's better safe than sorry to avoid errors
        std::string envvar = entry.substr(5, entry.find('=')-5); // Now we have USER_AGENT
        
        // Replace underscore (_) with dash (-)
        for(int i = 0; i < envvar.size(); ++i) {
          if(envvar[i] == '_') {
            envvar[i] = '-';
          }
        }

        // Lowercase
        std::transform(envvar.begin(), envvar.end(), envvar.begin(), __string_asciitolower__);

        // Bring the first letter of each word to uppercase
        envvar[0] = __string_asciitoupper__(envvar[0]);
        for(int i = 1; i < envvar.size(); ++i) {
          if(envvar[i-1] == '-') {
            envvar[i] = __string_asciitoupper__(envvar[i]);
          }
        }

        headers.push_back(std::make_pair(envvar, value));
      }
    }

    list_generated = true;
  }

  int get_header_index(std::string name) {
    for(int i = 0; i < headers.size(); ++i) {
      if(headers[i].first == name) return i;
    }
    return -1;
  }

  std::vector<std::pair<std::string, std::string>> headers;
  bool list_generated;
};

__request_headers__ request_headers;

void send_static(std::string filename) {
  std::string content;
  
  std::fstream file;
  file.open(filename, std::fstream::in);
  
  if(!file.is_open()) {
    // Failure to open the file
    std::cout << "Status: 500" << std::endl;
    std::cout << "Content-Type: text/plain" << std::endl << std::endl;

    std::cout << "500 Internal Server Error" << std::endl << std::endl;
    std::cout << "Could not open file: file not available for reading" << std::endl;
    return;
  }
  
  content.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

  file.close();

  std::cout << headers.format();
  std::cout << content << std::endl;
}

}  // namespace cgi

#endif  // CGILIB_INCLUDE_CGILIB_H_

