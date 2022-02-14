#include <iostream>
#include <fstream>

#include <lexer.h>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Invalid parameters!" << std::endl;
        return -1;
    }

    std::ifstream tests{argv[1]};

    if(!tests.is_open()) {
        std::cerr << "Unable to open '" << argv[1] << "'!" << std::endl;
        return -1;
    }

    lexer::lexer lex{tests};

    while(true) {
        lexer::token t = lex.next_unignored_token();
        std::cout << t.type << " -> " << t.identifier
                  << ", [" << t.pos.start_line << "," << t.pos.start_column << "] to [" << t.pos.end_line << "," << t.pos.end_column << "]" << std::endl;
        if(t.type == lexer::token::END_OF_FILE) break;
    }

    return 0;
}