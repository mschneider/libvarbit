CXXFLAGS = -g -O3 -I . -Wall -Wextra -Wformat=2 -Wold-style-cast -Winit-self -Wmissing-include-dirs -Wpointer-arith -Wredundant-decls -Wundef -Wwrite-strings
TEST_SOURCES = test/main.cc test/base_iterator.cc test/vector.cc test/const_iterator.cc test/reference.cc test/iterator.cc
TEST_OBJS = $(patsubst test/%.cc,build/%.o,$(TEST_SOURCES))
BENCHMARK_SOURCES = benchmark/sum.cc benchmark/random.cc benchmark/push_back.cc
BENCHMARK_OBJS = $(patsubst benchmark/%.cc,build/%.o,$(BENCHMARK_SOURCES))
BENCHMARK_BINARIES = $(patsubst benchmark/%.cc,build/benchmark_%,$(BENCHMARK_SOURCES))
ifneq ($(shell locate papi_version),)
	BENCHMARK_CXXFLAGS = -DUSE_PAPI
endif
PREFIX=/usr/local

all: $(BENCHMARK_BINARIES)
install:
	cp -R varbit $(PREFIX)/include

.PHONY: clean distclean
clean:
	$(RM) -r build
distclean: clean
	$(RM) -r $(PREFIX)/include/varbit

build/.created:
	mkdir -p build
	touch $@

build/dependencies: build/.created $(TEST_SOURCES) $(BENCHMARK_SOURCES)
	$(CXX) $(CXXFLAGS) -MM $(TEST_SOURCES) >build/test.d
	$(CXX) $(CXXFLAGS) $(BENCHMARK_CXXFLAGS) -MM $(BENCHMARK_SOURCES) >build/benchmark.d
	cat build/*.d | sed 's|.*\.o|build/&|g' >build/dependencies
include build/dependencies

style: tools/cpplint.py
	python tools/cpplint.py --filter=-legal,-readability/streams benchmark/* varbit/* test/*
tools/cpplint.py:
	mkdir -p tools
	wget -O $@ http://google-styleguide.googlecode.com/svn/trunk/cpplint/cpplint.py

benchmark: $(BENCHMARK_BINARIES)
	mkdir -p results
	build/benchmark_sum 100 >results/sum.100
	build/benchmark_random 100 >results/random.100
	build/benchmark_push_back 100 >results/push_back.100
build/benchmark_%: build/%.o
	$(CXX) $(CXXFLAGS) $(BENCHMARK_CXXFLAGS) -o $@ $<

check: build/test
	build/test
build/test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -lgtest -lgtest_main -o $@ $^

$(BENCHMARK_OBJS): build/.created
	$(CXX) $(CXXFLAGS) $(BENCHMARK_CXXFLAGS) -c -o $@ $(patsubst build/%.o,benchmark/%.cc,$@)

$(TEST_OBJS): build/.created
	$(CXX) $(CXXFLAGS) -c -o $@ $(patsubst build/%.o,test/%.cc,$@)

