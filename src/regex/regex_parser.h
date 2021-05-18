#pragma once

#include "regex_lexer.h"

namespace regex {
    class regex_parser {
        private:
            regex_lexer& lexer;
        public:
            regex_parser(regex_lexer& lexer);
            ~regex_parser();
    };
}