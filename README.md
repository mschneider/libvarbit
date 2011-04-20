# libvarbit
Data structures storing data with variable bit length.
Please note that libvarbit does no automatic sign extension for signed values,
you have to do it yourself: e.g using Sean Anderson's excellent
[Bit Twiddling Hacks](http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend).

## Dependencies
[googletest](http://code.google.com/p/googletest) (1.5)
[FAQ](http://code.google.com/p/googletest/wiki/FAQ)
[DOC1](http://code.google.com/p/googletest/wiki/Primer)
[DOC2](http://code.google.com/p/googletest/wiki/AdvancedGuide)

[PAPI](http://icl.cs.utk.edu/papi/) (4.1) (only needed for our benchmarks)

## Installation
To install the library execute `make install`.

## Tests
To run the tests execute `make check`.
Please make sure to run the tests before submitting changes.
Our code follows the
[Google C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml)
with the exception of using the iostreams library.
We encourage usage of the lint tool (execute: `make style`).

## Benchmarks
We implemented 3 synthetic benchmarks measuring sequential read, random read
and insertion against a plain `std::vector`.
To build the benchmarks execute `make`. To run those execute `make benchmark`.
