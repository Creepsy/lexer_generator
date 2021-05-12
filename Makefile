src = src/
build = build/
args = 

all: $(build)generator.o
	g++ $(build)generator.o -o generator

$(build)generator.o: $(src)generator.cpp
	g++ -c $(src)generator.cpp -o $(build)generator.o