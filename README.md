# libvarbit
Data structures storing data with variable bit length.

## Dependencies
CMake (2.8)
googletest (1.5)

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
