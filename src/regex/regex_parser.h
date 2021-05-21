#pragma once

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
        public:
            regex_parser(regex_lexer& lexer);
            ~regex_parser();
    };
}