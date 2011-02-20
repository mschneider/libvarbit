# libvarbit
Data structures storing data with variable bit length.
Please note that libvarbit does no automatic sign extension for signed values,
you have to do it yourself: e.g using Sean Anderson's excellent
[Bit Twiddling Hacks](http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend).

## Dependencies
[CMake](http://www.cmake.org) (2.8)
[FAQ](http://www.cmake.org/Wiki/CMake_FAQ)
[DOC](http://www.cmake.org/cmake/help/cmake-2-8-docs.html)

[googletest](http://code.google.com/p/googletest) (1.5)
[FAQ](http://code.google.com/p/googletest/wiki/FAQ)
[DOC1](http://code.google.com/p/googletest/wiki/Primer)
[DOC2](http://code.google.com/p/googletest/wiki/AdvancedGuide)

[PAPI](http://icl.cs.utk.edu/papi/) (4.1) (only needed for our benchmarks)

## Build
To build the benchmarks and tests simply execute:

	mkdir build
	cd build
	cmake ..
	make

To change the compilation flags edit
[CMakeLists.txt](https://github.com/mschneider/libvarbit/blob/master/CMakeLists.txt).
Afterwards execute `make rebuild_cache` and `make`.
	
## Installation
Follow the "Build" instructions and execute `make install`.

## Tests
Follow the "Build" instructions and execute `./tests` afterwards. Please make
sure to run the tests before submitting your changes. Our code follows the 
[Google C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
with the exception of using the iostreams library. We encourage usage of the
lint tool (execute: `make style`).

## Benchmarks
We implemented 3 synthetic benchmarks measuring sequential read, random read and
insertion against a plain `std::vector`.