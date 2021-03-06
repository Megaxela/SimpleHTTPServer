# Simple HTTP Server
It's pure C++17 single threaded HTTP server. 

## Dependencies
Only dependencies for this project are:
1. `ALogger` - my library for logging. 
2. `nlohmann/json` - for REST implementation.

All dependencies can be satisfied with `git submodule init`.  

## Build
It's CMake based project. It can be used as submodule.

Build instructions:

1. Clone repo: `git clone https://github.com/Megaxela/SimpleHTTPServer`.
1. Go into cloned repo: `cd SimpleHTTPServer`
    1. If you want to build tests: `git submodule update --init`
1. Create build folder: `mkdir build`
1. Go into build folder `cd build`
1. Setup project: `cmake <DEFINES_HERE> ..`.
    1. If you want to build tests or examples`--DSIMPLEHTTP_BUILD_TESTS=On` or `-DSIMPLEHTTP_BUILD_EXAMPLES=On`.
1. Build library: `cmake --build .`.

## Examples
Repository contains several examples. 
1. `RESTServer` - example usage of implemented REST server. Usage: `RESTServer <port>`

## License
<img align="right" src="https://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.