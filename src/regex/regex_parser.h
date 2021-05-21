#pragma once

#include <vector>

#include "regex_lexer.h"
#include "ast.h"

namespace regex {
    class regex_parser {
        private:
            regex_lexer& lexer;
            token curr;

            void next();
            bool accept(const std::vector<token::token_type>& types);
            bool accept(const token::token_type type);
            void expect(const token::token_type type);
            void expect(const std::vector<token::token_type>& types);

            ast::branch* parse_option();
            ast::branch* parse_sequence();
            ast::branch* parse_quantifier();
            ast::branch* parse_group();
            ast::branch* parse_character();
            ast::branch* parse_character_set();

            std::vector<ast::char_range> parse_special_character();
        public:
            regex_parser(regex_lexer& lexer);
            ast::branch* parse_regex();
            ~regex_parser();
    };
}