#include <iostream>
#include <fstream>

#include "lexer.h"

int main() {
    std::ifstream tests{"test.txt"};

    if(!tests.is_open()) {
        std::cerr << "Unable to open test.txt!" << std::endl;
        return -1;
    }

    lexer lex{tests};

    while(true) {
        token t = lex.next_token();
        std::cout << t.type << " -> " << t.identifier
                  << ", [" << t.pos.start_line << "," << t.pos.start_column << "] to [" << t.pos.end_line << "," << t.pos.end_column << "]" << std::endl;
        if(t.type == token::UNDEFINED) break;
    }

    return 0;
}