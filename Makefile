src = src/
build = build/
args = 

all: $(build)generator.o $(build)regex_lexer.o $(build)regex_parser.o
	g++ $(build)generator.o -o generator $(build)regex_lexer.o $(build)regex_parser.o

$(build)generator.o: $(src)generator.cpp $(src)regex/regex_lexer.h $(src)regex/regex_parser.h
	g++ -c $(src)generator.cpp -o $(build)generator.o

$(build)regex_lexer.o: $(src)regex/regex_lexer.cpp $(src)regex/regex_lexer.h
	g++ -c $(src)regex/regex_lexer.cpp -o $(build)regex_lexer.o

$(build)regex_parser.o: $(src)regex/regex_parser.cpp $(src)regex/regex_parser.h
	g++ -c $(src)regex/regex_parser.cpp -o $(build)regex_parser.o