#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

#include "regex/regex_lexer.h"
#include "regex/regex_parser.h"
#include "automata.h"

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Missing required parameters ... exit" << std::endl;
        return -1;
    }
    
    std::ifstream rules{argv[1]};
    if(!rules.is_open()) {
        std::cerr << "Unable to open the rules file!" << std::endl;
        return -1;
    }

    std::vector<std::pair<std::string, ast::branch*>> token_rules;
    std::string token_rule;
    while(rules.good()) {
        std::getline(rules, token_rule);
        size_t identifier_size = token_rule.find(' ');
        std::string identifier = token_rule.substr(0, identifier_size);
        std::stringstream regex{token_rule.substr(identifier_size + 1)};

        regex::regex_lexer lexer{regex};
        regex::regex_parser parser{lexer};

        token_rules.push_back(std::make_pair(identifier, parser.parse_regex()));
    }
    automata::automaton nfa = automata::automaton::nfa_from_token_rules(token_rules);

    for(const std::pair<std::string, ast::branch*>& rule : token_rules) delete rule.second;
    
    automata::automaton dfa  = automata::automaton::dfa_from_nfa(nfa);

    std::cout << "NFA: " << nfa.to_string() << '\n' << std::endl;
    std::cout << "DFA: " << dfa.to_string() << std::endl;

    return 0;
}