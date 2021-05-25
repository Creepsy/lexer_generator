#include <iostream>
#include <sstream>
#include <utility>
#include <string>

#include "regex/regex_lexer.h"
#include "regex/regex_parser.h"
#include "automata.h"

int main() {
    std::stringstream integers_regex("\\d+");
    std::stringstream floats_regex("[+-]?(\\d*\\.\\d+|\\d+\\.)([eE]([+-]?\\d+))?");

    regex::regex_lexer integers_lexer{integers_regex};
    regex::regex_lexer floats_lexer{floats_regex};

    regex::regex_parser integers_parser{integers_lexer};
    regex::regex_parser floats_parser{floats_lexer};

    ast::branch* integers = integers_parser.parse_regex();
    ast::branch* floats = floats_parser.parse_regex();

    std::cout << "integers: " << integers->to_string() << '\n' << std::endl;
    std::cout << "floats: " << floats->to_string() << '\n' << std::endl;

    std::vector<std::pair<std::string, ast::branch*>> token_rules {
        std::make_pair("INTEGER", integers), std::make_pair("FLOAT", floats)
    };

    automata::automaton nfa = automata::automaton::nfa_from_token_rules(token_rules);

    delete integers;
    delete floats;

    automata::automaton dfa  = automata::automaton::dfa_from_nfa(nfa);

    std::cout << "NFA: " << nfa.to_string() << '\n' << std::endl;
    std::cout << "DFA: " << dfa.to_string() << std::endl;

    return 0;
}