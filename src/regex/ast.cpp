#include "ast.h"

#include <cstddef>

using namespace ast;

branch::branch() {
}

branch::~branch() {
}



sequence_branch::sequence_branch(bool option) : branch(), option(option) {

}

sequence_branch::sequence_branch(std::vector<branch*>& elements, bool is_option) : branch(), elements(elements), option(option) {

}

void sequence_branch::add_element(branch* element) {
    this->elements.push_back(element);
}

bool sequence_branch::is_option() {
    return this->option;
}

std::string sequence_branch::to_string() {
    std::string as_str = (this->option) ? "Option{" : "Sequence{";

    for(size_t o = 0; o < this->elements.size(); o++) {
        if(o != 0) as_str += ", ";
        as_str += this->elements[o]->to_string();
    }

    return as_str + "}";
}

sequence_branch::~sequence_branch() {
    for(branch* element : this->elements) {
        if(element) delete element;
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



character_branch::character_branch() {

}

std::string character_branch::to_string() {

}

character_branch::~character_branch() {
    
}