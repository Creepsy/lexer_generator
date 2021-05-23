src = src/
build = build/
args = 

all: $(build)generator.o $(build)regex_lexer.o $(build)regex_parser.o $(build)ast.o $(build)nfa.o
	g++ $(build)generator.o -o generator $(build)regex_lexer.o $(build)regex_parser.o $(build)ast.o $(build)nfa.o

$(build)generator.o: $(src)generator.cpp $(src)regex/regex_lexer.h $(src)regex/regex_parser.h
	g++ -c $(src)generator.cpp -o $(build)generator.o

$(build)regex_lexer.o: $(src)regex/regex_lexer.cpp $(src)regex/regex_lexer.h
	g++ -c $(src)regex/regex_lexer.cpp -o $(build)regex_lexer.o

$(build)regex_parser.o: $(src)regex/regex_parser.cpp $(src)regex/regex_parser.h $(src)regex/regex_lexer.h $(src)regex/ast.h
	g++ -c $(src)regex/regex_parser.cpp -o $(build)regex_parser.o

$(build)ast.o: $(src)regex/ast.cpp $(src)regex/ast.h
	g++ -c $(src)regex/ast.cpp -o $(build)ast.o

$(build)nfa.o: $(src)nfa.cpp $(src)nfa.h
	g++ -c $(src)nfa.cpp -o $(build)nfa.o