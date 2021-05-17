src = src/
build = build/
args = 

all: $(build)generator.o $(build)regex_lexer.o
	g++ $(build)generator.o -o generator $(build)regex_lexer.o

$(build)generator.o: $(src)generator.cpp $(src)regex/regex_lexer.h
	g++ -c $(src)generator.cpp -o $(build)generator.o

$(build)regex_lexer.o: $(src)regex/regex_lexer.cpp $(src)regex/regex_lexer.h
	g++ -c $(src)regex/regex_lexer.cpp -o $(build)regex_lexer.o