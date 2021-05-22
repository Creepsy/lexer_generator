#include <iostream>
#include <sstream>

#include "regex/regex_lexer.h"
#include "regex/regex_parser.h"

int main() {
    std::stringstream to_parse("((([^a-hklq\\W]))){6,}");

    regex::regex_lexer lexer(to_parse);
    regex::regex_parser parser(lexer);

    ast::branch* out = parser.parse_regex();
    std::cout << out->to_string() << std::endl;

    delete out;

    /*while(lexer.good()) {
        std::cout << lexer.next_token() << std::endl;
    }*/

    return 0;
}