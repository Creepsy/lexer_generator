src = src/
build = build/
args =

all: $(build)generator.o $(build)regex_lexer.o $(build)regex_parser.o $(build)ast.o $(build)automata.o
	g++ $(build)generator.o -o generator $(build)regex_lexer.o $(build)regex_parser.o $(build)ast.o $(build)automata.o $(args)

$(build)generator.o: $(src)generator.cpp $(src)regex/regex_lexer.h $(src)regex/regex_parser.h $(src)templates.h $(src)regex/regex_lexer.h $(src)automata.h $(src)regex/regex_parser.h
	g++ -c $(src)generator.cpp -o $(build)generator.o $(args)

$(build)regex_lexer.o: $(src)regex/regex_lexer.cpp $(src)regex/regex_lexer.h
	g++ -c $(src)regex/regex_lexer.cpp -o $(build)regex_lexer.o $(args)

$(build)regex_parser.o: $(src)regex/regex_parser.cpp $(src)regex/regex_parser.h $(src)regex/regex_lexer.h $(src)regex/ast.h
	g++ -c $(src)regex/regex_parser.cpp -o $(build)regex_parser.o $(args)

$(build)ast.o: $(src)regex/ast.cpp $(src)regex/ast.h
	g++ -c $(src)regex/ast.cpp -o $(build)ast.o $(args)

$(build)automata.o: $(src)automata.cpp $(src)automata.h
	g++ -c $(src)automata.cpp -o $(build)automata.o $(args)