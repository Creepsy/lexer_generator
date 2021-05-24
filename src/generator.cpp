#include <iostream>
#include <sstream>
#include <utility>
#include <string>

#include "regex/regex_lexer.h"
#include "regex/regex_parser.h"
#include "automata.h"

int main() {
    std::stringstream number_regex("2\\.(\\d\\d)|3\\.[a-z\\d]\\d{2}");
    std::stringstream var_regex("VAR\\d{3,5}");

    regex::regex_lexer number_lexer{number_regex};
    regex::regex_lexer var_lexer{var_regex};

    regex::regex_parser number_parser{number_lexer};
    regex::regex_parser var_parser{var_lexer};

    ast::branch* number = number_parser.parse_regex();
    ast::branch* var = var_parser.parse_regex();

    std::cout << "Number: " << number->to_string() << std::endl;
    std::cout << "Variable: " << var->to_string() << std::endl;

    std::vector<std::pair<std::string, ast::branch*>> token_rules {
        std::make_pair("NUMBER", number), std::make_pair("VARIABLE", var)
    };

    automata::automaton machine = automata::automaton::from_token_rules(token_rules);

    delete number;
    delete var;

    std::cout << "NFA: " << machine.to_string() << std::endl;

    return 0;
}