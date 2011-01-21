# libvarbit
Data structures storing data with variable bit length.

## Dependencies
[CMake](http://www.cmake.org) (2.8)
[FAQ](http://www.cmake.org/Wiki/CMake_FAQ)
[DOC](http://www.cmake.org/cmake/help/cmake-2-8-docs.html)

[googletest](http://code.google.com/p/googletest) (1.5)
[FAQ](http://code.google.com/p/googletest/wiki/FAQ)
[DOC1](http://code.google.com/p/googletest/wiki/Primer)
[DOC2](http://code.google.com/p/googletest/wiki/AdvancedGuide)

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
Follow the "Build" instructions and execute `make install` instead of `make`.

## Tests
Follow the "Build" instructions and execute `make test` afterwards.

## Benchmarks
Follow the "Build" instructions and execute `benchmark_sum` afterwards.
