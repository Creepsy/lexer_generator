cmake_minimum_required(VERSION 3.10)

project(lex_gen_test)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/lexer.cpp ${CMAKE_BINARY_DIR}/lexer.h
    COMMAND generator ${CMAKE_CURRENT_SOURCE_DIR}/lexer_rules.txt ${CMAKE_BINARY_DIR}/ lexer
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexer_rules.txt
)

set(SOURCE_FILES
    ${CMAKE_BINARY_DIR}/lexer.cpp
    
    ${CMAKE_BINARY_DIR}/lexer.h
)

add_executable(test_lexer test.cpp ${SOURCE_FILES})

target_include_directories(test_lexer PRIVATE ${CMAKE_BINARY_DIR})