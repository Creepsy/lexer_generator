#include "ast.h"

#include <cstddef>

using namespace ast;

branch::branch() {
}

branch::~branch() {
}



option_branch::option_branch() : branch() {

}

option_branch::option_branch(std::vector<branch*>& options) : branch(), options(options) {

}

void option_branch::add_option(branch* option) {
    this->options.push_back(option);
}

std::string option_branch::to_string() {
    std::string as_str = "Option{";

    for(size_t o = 0; o < this->options.size(); o++) {
        if(o != 0) as_str += ", ";
        as_str += this->options[o]->to_string();
    }

    return as_str + "}";
}

option_branch::~option_branch() {
    for(branch* option : this->options) {
        if(option) delete option;
    }
}



quantifier_branch::quantifier_branch() {

}

std::string quantifier_branch::to_string() {

}

quantifier_branch::~quantifier_branch() {

}



sequence_branch::sequence_branch() {

}
std::string sequence_branch::to_string() {
    
}

sequence_branch::~sequence_branch() {

}



character_branch::character_branch() {

}

std::string character_branch::to_string() {

}

character_branch::~character_branch() {
    
}