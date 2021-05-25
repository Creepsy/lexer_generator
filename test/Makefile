all: build/test.o build/lexer.o
	g++ build/test.o -o test build/lexer.o

build/test.o: test.cpp lexer.h
	g++ -c test.cpp -o build/test.o

build/lexer.o: lexer.cpp lexer.h
	g++ -c lexer.cpp -o build/lexer.o