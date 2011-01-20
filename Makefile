benchmark: build
	g++ -O3 -o build/sum benchmarks/sum.cpp 

clean:
	rm -rf build

rebuild: clean
	make

build:
	mkdir -p build