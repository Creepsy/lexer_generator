#include "regex_lexer.h"

using namespace regex;

regex_lexer::regex_lexer(std::istream& input) : input(input) {

}

token regex_lexer::next_token() {
    char next = this->input.get();

    if(this->input.eof()) return token{token::END_OF_FILE, "EOF"};
    if(this->input.fail()) return token{token::UNDEFINED, "UNDEFINED"};

    switch(next) {
        case '\\':
            return this->next_escaped();
        case '.':
            return token{token::ANY_CHAR, "."};
        case '+':
            return token{token::PLUS, "+"};
        case '-':
            return token{token::MINUS, "-"};
        case '*':
            return token{token::STAR, "*"};
        case '?':
            return token{token::QMARK, "?"};
        case '|':
            return token{token::OR, "|"};
        case '{':
            return token{token::BRACE_OPEN, "{"};
        case '}':
            return token{token::BRACE_CLOSE, "}"};
        case '[':
            return token{token::SQBRACKET_OPEN, "["};
        case ']':
            return token{token::SQBRACKET_CLOSE, "]"};
        case '(':
            return token{token::BRACKET_OPEN, "("};
        case ')':
            return token{token::BRACKET_CLOSE, ")"};
        case '^':
            return token{token::NOT, "^"};
        case ',':
            return token{token::COMMA, ","};
        default:
            return token{token::CHAR, std::string(1, next)};
    }
}

bool regex_lexer::fail() {
    return this->input.fail();
}

bool regex_lexer::good() {
    return this->input.good();
}

regex_lexer::~regex_lexer() {
    
}

// private

token regex_lexer::next_escaped() {
    char next = this->input.get();

    switch(next) {
        case 't':
            return token{token::CHAR, "\t"};
        case 'n':
            return token{token::CHAR, "\n"};
        case 'r':
            return token{token::CHAR, "\r"};
        case '?':
            return token{token::CHAR, "?"};
        case 'a':
            return token{token::CHAR, "\a"};
        case 'b':
            return token{token::CHAR, "\b"};
        case 'f':
            return token{token::CHAR, "\f"};
        case 'v':
            return token{token::CHAR, "\v"};
        case 'w':
            return token{token::WORD, "WORD"};
        case 'd':
            return token{token::DIGIT, "DIGIT"};
        case 's':
            return token{token::WSPACE, "WHITESPACE"};
        case 'W':
            return token{token::NWORD, "NO WORD"};
        case 'D':
            return token{token::NDIGIT, "NO DIGIT"};
        case 'S':
            return token{token::NWSPACE, "NO WHITESPACE"};
        default:
            return token{token::CHAR, std::string(1, next)};
    }
}

// operators

std::ostream& operator<<(std::ostream& stream, const token& t) {
    return stream << "token{id: " << t.type << ", ident: '" << t.identifier << "'}"; 
}