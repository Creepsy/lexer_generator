#include "regex_parser.h"

using namespace regex;

regex_parser::regex_parser(regex_lexer& lexer) : lexer(lexer) {
}

regex_parser::~regex_parser() {
}