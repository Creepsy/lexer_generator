#include <iostream>
#include <sstream>

#include "regex/regex_lexer.h"

int main() {
    std::stringstream to_parse("([A-Z])\\w+\\?");

    regex::regex_lexer lexer(to_parse);

    while(lexer.good()) {
        std::cout << lexer.next_token() << std::endl;
    }

    return 0;
}