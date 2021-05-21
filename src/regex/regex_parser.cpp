#include "regex_parser.h"

#include <stdexcept>

using namespace regex;

regex_parser::regex_parser(regex_lexer& lexer) : lexer(lexer) {
    this->next();
}

regex_parser::~regex_parser() {
}

// private

void regex_parser::next() {
    this->curr = this->lexer.next_token();
}

bool regex_parser::accept(const std::vector<token::token_type>& types) {
    for(const token::token_type type : types) {
        if(this->accept(type)) return true;
    }
    return false;
}

bool regex_parser::accept(const token::token_type type) {
    return this->curr.type == type;
}

void regex_parser::expect(const token::token_type type) {
    if(!this->accept(type)) throw std::runtime_error("Invalid token!");
}

void regex_parser::expect(const std::vector<token::token_type>& types) {
    if(!this->accept(types)) throw std::runtime_error("Invalid token!");
}