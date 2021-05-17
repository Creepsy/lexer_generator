#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <cstddef>

namespace regex {

    struct token {
        enum token_type {
            CHAR,
            ANY_CHAR,
            WORD,
            DIGIT,
            WSPACE,
            NWORD,
            NDIGIT,
            NWSPACE,
            SQBRACKET_OPEN,
            SQBRACKET_CLOSE,
            BRACKET_OPEN,
            BRACKET_CLOSE,
            BRACE_OPEN,
            BRACE_CLOSE,
            STAR,
            PLUS,
            MINUS,
            QMARK,
            COMMA,
            OR,
            NOT,
            END_OF_FILE,
            UNDEFINED
        };

        token_type type;
        std::string identifier;
    };

    class regex_lexer {
        private:
            std::istream& input;

            token next_escaped();
        public:
            regex_lexer(std::istream& input);
            token next_token();
            bool fail();
            bool good();
            ~regex_lexer();
    };
}

std::ostream& operator<<(std::ostream& stream, const regex::token& t);