# cgilib

![travis-build](https://img.shields.io/travis/com/AdrianGjerstad/cgilib?logo=travis&style=flat-square)
![license-badge](https://img.shields.io/github/license/AdrianGjerstad/cgilib?style=flat-square&logo=github)
![platform-linux](https://img.shields.io/badge/platform-linux-blue?style=flat-square&logo=linux)
![std-c++17](https://img.shields.io/badge/std-c++17-blue?style=flat-square&logo=c%2b%2b)

CGILib is a C++ library built out of the sheer frustration of having to build all of that barebones code that you need to get information from the server. It is a single-header Common Gateway Interface Library meant to squeeze every ounce of data out of the environment and feed it to you when you want.

## Installation

Unfortunately, as this project has yet to receive a release, there are no installation instructions. Just take the header file at the root of this repo, and put it in (for \*nix style systems) `/usr/local/include`.

When the first release is posted this README will have a one-liner install command.

## Usage

```cpp
#include <cstdlib>   // srand, rand
#include <ctime>     // time
#include <cgilib.h>  // CGI Library
#include <iostream>  // endl

int main(int argc, char** argv) {
  // Set up pseudo-random generation
  srand(time(NULL));

  // Set up a basic header
  cgi::headers.add_header("Content-Type", "text/plain");

  cgi::out << "I generated a random number! " << (rand()%10+1) << std::endl;
  
  cgi::send_response();
  return 0;
}
```

See more examples in the `/examples` directory.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

If an issue is already open, and you would like to tackle it, mention that you would like to, and we can collaborate
from there.

Please make sure to test your additions, and, if you come up with a new test, put it in the `/test` directory.

## License

[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
