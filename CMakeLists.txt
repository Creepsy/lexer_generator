cmake_minimum_required(VERSION 3.10)

project(lexer_generator)

set(SOURCE_FILES
    src/regex/ast.cpp
    src/regex/regex_lexer.cpp
    src/regex/regex_parser.cpp
    src/automata.cpp

    src/regex/ast.h
    src/regex/regex_lexer.h
    src/regex/regex_parser.h
    src/automata.h
)

add_executable(lexer_generator src/generator.cpp ${SOURCE_FILES})

add_subdirectory(test)