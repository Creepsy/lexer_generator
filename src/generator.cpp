#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>
#include <cstring>

#include "regex/regex_lexer.h"
#include "regex/regex_parser.h"
#include "automata.h"
#include "templates.h"

void write_dfa_to_source(const automata::automaton& dfa, std::ofstream& stream, bool use_ranges);
void write_ignored_tokens_to_source(std::ofstream& stream, const std::vector<std::string>& ignored_tokens);

int main(int argc, char* argv[]) {
    if(argc < 4) {
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
    std::vector<std::string> ignored_tokens;

    while(rules.good()) {
        std::getline(rules, token_rule);
        size_t identifier_size = token_rule.find(' ');
        std::string identifier = token_rule.substr(0, identifier_size);
        std::stringstream regex{token_rule.substr(identifier_size + 1)};

        regex::regex_lexer lexer{regex};
        regex::regex_parser parser{lexer};

        if(identifier[0] == '$') {
            identifier = identifier.substr(1);
            ignored_tokens.push_back(identifier);
        }

        token_rules.push_back(std::make_pair(identifier, parser.parse_regex()));
    }

    automata::automaton nfa = automata::automaton::nfa_from_token_rules(token_rules);
    for(const std::pair<std::string, ast::branch*>& rule : token_rules) delete rule.second;
    
    automata::automaton dfa  = automata::automaton::dfa_from_nfa(nfa);

    std::string header_path = std::string(argv[2]) + argv[3];
    std::string source_path = std::string(argv[2]) + argv[4];

    std::ofstream lexer_header{header_path, std::ios::trunc};
    std::ofstream lexer_source{source_path, std::ios::trunc};

    if(!lexer_header.is_open() || !lexer_source.is_open()) {
        std::cerr << "Unable to create header or source!" << std::endl;
        return -1;
    }

    lexer_header << templates::LEXER_HEADER[0];
    lexer_source << templates::LEXER_SOURCE[0];
    lexer_source << argv[3];
    lexer_source << templates::LEXER_SOURCE[1];


    lexer_header << "\t\tEND_OF_FILE,\n";
    lexer_header << "\t\tUNDEFINED,\n";

    for(size_t r = 0; r < token_rules.size(); r++) {
        lexer_header << "\t\t" << token_rules[r].first;
        if(r != token_rules.size() - 1) lexer_header << ",";
        lexer_header << '\n';
    }

    write_ignored_tokens_to_source(lexer_source, ignored_tokens);

    lexer_source << templates::LEXER_SOURCE[2];

    if(argc == 6 && std::string(argv[5]) == "no_ranges") {
        write_dfa_to_source(dfa, lexer_source, false);
    } else {
        write_dfa_to_source(dfa, lexer_source, true);
    } 

    lexer_header << templates::LEXER_HEADER[1];
    lexer_source << templates::LEXER_SOURCE[3];

    lexer_header.close();
    lexer_source.close();

    std::cout << "Header and source file got successfully created!" << std::endl;

    return 0;
}

void write_dfa_to_source(const automata::automaton& dfa, std::ofstream& stream, bool use_ranges) {
    for(size_t n = 0; n < dfa.get_nodes().size(); n++) {
        const automata::node curr = dfa.get_nodes()[n];
        stream << "\t\t\tcase " << n << ":\n";

        stream << "\t\t\t\tswitch(this->curr) {\n";

        for(const automata::connection& c : curr.connections) {
            if(use_ranges) {
                for(const ast::char_range& req : c.requirements) {
                    if(req.start == req.end) {
                        stream << "\t\t\t\t\tcase " + std::to_string((int)req.start) + ":\n";
                    } else {
                        stream << "\t\t\t\t\tcase " + std::to_string((int)req.start) + " ... " + std::to_string((int)req.end) + ":\n";
                    }
                }
                stream << "\t\t\t\t\t\tstate = " + std::to_string(c.node_id) + ";\n";
                stream << "\t\t\t\t\t\tbreak;\n";
            } else {
                for(const ast::char_range& req : c.requirements) {
                    for(char c = req.start; c <= req.end; c++) {
                        stream << "\t\t\t\t\tcase " + std::to_string((int)c) + ":\n";
                    }
                }
                stream << "\t\t\t\t\t\tstate = " + std::to_string(c.node_id) + ";\n";
                stream << "\t\t\t\t\t\tbreak;\n";
            }
        }

        if(curr.root_tag != "") {
            stream << "\t\t\t\t\tdefault:\n";
            stream << "\t\t\t\t\t\treturn token{identifier, token::" + curr.root_tag + ", position{start_line, start_column, this->curr_line, this->curr_column}};\n";
        } else {
            stream << templates::ERROR_CHAR;
        }

        stream << "\t\t\t\t}\n";
        stream << "\t\t\t\tbreak;\n";
    }

    stream << templates::ERROR_NODE;
}

void write_ignored_tokens_to_source(std::ofstream& stream, const std::vector<std::string>& ignored_tokens) {
    for(size_t t = 0; t < ignored_tokens.size(); t++) {
        stream << "\ttoken::" << ignored_tokens[t];
        if(t != ignored_tokens.size() - 1) stream << ',';
        stream << '\n';
    }
}