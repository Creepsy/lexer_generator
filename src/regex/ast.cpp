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



character_set_branch::character_set_branch(const std::vector<char_range>& characters, const bool negated)
 : branch(), characters(characters), negated(negated) {

}

character_set_branch::character_set_branch(const bool negated) : branch(), characters(), negated(negated) {

}

bool character_set_branch::is_negated() {
    return this->negated;
}

void character_set_branch::add_character_range(const char_range range) {
    this->characters.push_back(range);
}

void character_set_branch::add_character_ranges(const std::vector<char_range>& characters) {
    this->characters.insert(this->characters.end(), characters.begin(), characters.end());
}

const std::vector<char_range>& character_set_branch::get_characters() {
    return this->characters;
}

std::string character_set_branch::to_string() {
    std::string as_str = "Character_Set{chars: ";

    for(const char_range c : this->characters) {
        if(c.negated) as_str += "!";
        if(c.start == c.end) {
            as_str += c.start;
        } else {
            as_str += "[" + std::string(1, c.start) + "-" + std::string(1, c.end) + "]";
        }
    }

    return as_str + "}";
}

character_set_branch::~character_set_branch() {
}