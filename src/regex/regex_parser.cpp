#include "regex_parser.h"

#include <stdexcept>

using namespace regex;

const std::vector<token::token_type> SPECIAL_CHARACTERS {
    token::ANY_CHAR,
    token::WORD,
    token::DIGIT,
    token::WSPACE,
    token::NWORD,
    token::NDIGIT,
    token::NWSPACE
};

regex_parser::regex_parser(regex_lexer& lexer) : lexer(lexer) {
    this->next();
}

ast::branch* regex_parser::parse_regex() {
   /* if(this->lexer.fail()) return nullptr;

    return this->parse_option();*/
    
    return this->parse_character();
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

ast::branch* regex_parser::parse_option() {
   // ast::branch* sequence = this->parse_sequence();
}

ast::branch* regex_parser::parse_sequence() {
   // ast::branch* quantifer = this->parse_quantifier();
}

ast::branch* regex_parser::parse_quantifier() {
   // ast::branch* character_set = this->parse_group();
}

ast::branch* regex_parser::parse_group() {
   // ast::branch* character_set = this->parse_character_set();
}

ast::branch* regex_parser::parse_character() {
    if(this->accept(token::SQBRACKET_OPEN)) {
        return this->parse_character_set();
    } else {
        ast::character_set_branch* set;
        if(this->accept(SPECIAL_CHARACTERS)) {
                set = new ast::character_set_branch(this->parse_special_character(), false);
        } else {
            if(this->curr.identifier.size() != 1) throw std::runtime_error("Invalid character!");
            set = new ast::character_set_branch(std::vector<ast::char_range>{ast::char_range{this->curr.identifier[0], this->curr.identifier[0]}}, false);
        }

        this->next();
        return set;
    }
}

ast::branch* regex_parser::parse_character_set() {
    this->expect(token::SQBRACKET_OPEN);
    this->next();

    ast::character_set_branch* set;
    if(this->accept(token::NOT)) {
        set = new ast::character_set_branch(true);
        this->next();
    } else {
        set = new ast::character_set_branch(false);
    }

    while(!this->accept(token::SQBRACKET_CLOSE) && this->lexer.good()) {
        if(this->accept(SPECIAL_CHARACTERS)) {
            set->add_character_ranges(this->parse_special_character());
            this->next();
        } else {
            if(this->curr.identifier.size() != 1) throw std::runtime_error("Invalid character!");
            char c = this->curr.identifier[0];
            
            this->next();
            if(this->accept(token::MINUS)) {
                this->next();
                if(this->curr.identifier.size() != 1) throw std::runtime_error("Invalid character!");
                char end = this->curr.identifier[0];
                this->next();

                if(end < c) throw std::runtime_error("Invalid character range!");
                set->add_character_range(ast::char_range{c, end});
            } else {
                set->add_character_range(ast::char_range{c, c});
            }
        }
    }

    this->expect(token::SQBRACKET_CLOSE);
    this->next();

    return set;
}

std::vector<ast::char_range> regex_parser::parse_special_character() {
    switch(this->curr.type) {
        case token::ANY_CHAR:
            return std::vector<ast::char_range> {ast::char_range{'\n', '\n', true}};
        case token::WORD:
        case token::NWORD:
            {
                bool neg = !(this->curr.type == token::WORD);
                return std::vector<ast::char_range> {
                    ast::char_range{'0', '9', neg}, ast::char_range{'a', 'z', neg}, ast::char_range{'A', 'Z', neg}, ast::char_range{'_', '_', neg}
                };
            }
        case token::DIGIT:
        case token::NDIGIT:
            {
                bool neg = !(this->curr.type == token::WORD);
                return std::vector<ast::char_range> {ast::char_range{'0', '9', neg}};
            }
        case token::WSPACE:
        case token::NWSPACE:
            {
                bool neg = !(this->curr.type == token::WORD);
                return std::vector<ast::char_range> {
                    ast::char_range{' ', ' ', neg}, ast::char_range{'\t', '\t', neg}, ast::char_range{'\r', '\r', neg},
                    ast::char_range{'\n', '\n', neg}, ast::char_range{'\v', '\v', neg}, ast::char_range{'\f', '\f', neg}
                };
            }
        default:
            throw std::runtime_error("Invalid special character!");
    }

}