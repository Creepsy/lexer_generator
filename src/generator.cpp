#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

#include "regex/regex_lexer.h"
#include "regex/regex_parser.h"
#include "automata.h"
#include "templates.h"

int main(int argc, char* argv[]) {
    if(argc != 4) {
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

    std::ofstream lexer_header{argv[2], std::ios::trunc};
    std::ofstream lexer_source{argv[3], std::ios::trunc};

    if(!lexer_header.is_open() || !lexer_source.is_open()) {
        std::cerr << "Unable to create header or source!" << std::endl;
        return -1;
    }

    lexer_header << templates::LEXER_HEADER[0];
    lexer_source << templates::LEXER_SOURCE[0];

    for(size_t r = 0; r < token_rules.size(); r++) {
        lexer_header << "\t\t" << token_rules[r].first;
        if(r != token_rules.size() - 1) lexer_header << ",";
        lexer_header << '\n';
    }

    lexer_header << templates::LEXER_HEADER[1];
    lexer_source << templates::LEXER_SOURCE[1];

    lexer_header.close();
    lexer_source.close();

    std::cout << "Header and source file got successfully created!" << std::endl;

    return 0;
}