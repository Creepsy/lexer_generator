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



quantifier_branch::quantifier_branch(branch* child, const size_t min, const size_t max, const bool limited)
 : branch(), child(child), min(min), max(max), limited(limited) {

}

size_t quantifier_branch::get_minimum() {
    return this->min;
}

size_t quantifier_branch::get_maximum() {
    return this->max;
}

bool quantifier_branch::is_limited() {
    return this->limited;
}

std::string quantifier_branch::to_string() {
    if(!this->limited) return "Quantifier{min: " + std::to_string(this->min) + ", max: NO LIMIT" + ", obj: " + this->child->to_string() + "}";
    return "Quantifier{min: " + std::to_string(this->min) + ", max: " + std::to_string(this->max) + ", obj: " + this->child->to_string() + "}";
}

quantifier_branch::~quantifier_branch() {
    if(this->child) delete this->child;
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