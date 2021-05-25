#include "ast.h"

#include <cstddef>
#include <algorithm>

using namespace ast;

std::vector<char_range> char_range::intersection(const char_range& other) const {
    if(!this->negated && !other.negated) {
        if(this->start > other.end || other.start > this->end) return std::vector<char_range>{};

        return std::vector<char_range>{char_range{std::max(this->start, other.start), std::min(this->end, other.end)}};
    } else {
        std::vector<char_range> this_no_neg = this->without_negation();
        std::vector<char_range> other_no_neg = other.without_negation();

        std::vector<char_range> intersections;

        for(char_range c0 : this_no_neg) {
            for(char_range c1 : other_no_neg) {
                std::vector<char_range> new_intersections = c0.intersection(c1);
                intersections.insert(intersections.end(), new_intersections.begin(), new_intersections.end());
            }
        }

        return char_range::simplify(intersections);
    }
}

std::vector<char_range> char_range::substract(const char_range& other) const {
    std::vector<char_range> result = this->without_negation();
    std::vector<char_range> intersections = this->intersection(other);

    for(char_range& intersect : intersections) {
        std::vector<char_range> new_ranges;
        for(size_t c = 0; c < result.size(); c++) {
            char_range& range = result[c];
            if(intersect.start == range.start && intersect.end == range.end) {
                result.erase(result.begin() + c);
                c--;
            } else if(intersect.start > range.start && intersect.end == range.end) {
                range.end = (char)(intersect.start - 1);
            } else if(intersect.end < range.end && intersect.start == range.start) {
                range.start = (char)(intersect.end + 1);
            } else if(intersect.start > range.start && intersect.end < range.end) {
                new_ranges.push_back(char_range{range.start, (char)(intersect.start - 1)});
                new_ranges.push_back(char_range{(char)(intersect.end + 1), range.end});
                result.erase(result.begin() + c);
                c--;
            }
        }
        result.insert(result.end(), new_ranges.begin(), new_ranges.end());
    }

    return char_range::simplify(result);
}

std::vector<char_range> char_range::without_negation() const {
    if(!this->negated) return std::vector<char_range>{*this};

    std::vector<char_range> ranges;

    if(this->start > 0) {
        ranges.push_back(char_range{(char)0, (char)(this->start - 1)});
    }

    if(this->end < 127) {
        ranges.push_back(char_range{(char)(this->end + 1), (char)127});
    }

    return ranges;
}

std::vector<char_range> char_range::simplify(std::vector<char_range> ranges) {
    std::vector<char_range> no_neg;

    for(const char_range& c : ranges) {
        if(c.negated) {
            std::vector<char_range> new_ranges = c.without_negation();
            no_neg.insert(no_neg.end(), new_ranges.begin(), new_ranges.end());
        } else {
            no_neg.push_back(c);
        }
    }

    std::sort(no_neg.begin(), no_neg.end(), [](const char_range& first, const char_range& second) -> bool {
        return first.start < second.start;
    });

    std::vector<char_range> grouped;

    for(const char_range& range : no_neg) {
        if(grouped.empty() || grouped.back().end < range.start - 1) {
            grouped.push_back(range);
        } else if(grouped.back().end < range.end) {
            grouped.back().end = range.end;
        }
    }

    return grouped;
}

std::vector<char_range> char_range::group_intersection(const std::vector<char_range>& first, const std::vector<char_range>& second) {
    std::vector<char_range> intersections;

    for(const char_range& f : first) {
        for(const char_range& s : second) {
            std::vector<char_range> intersection = f.intersection(s);
            intersections.insert(intersections.end(), intersection.begin(), intersection.end());
        }
    }

    return char_range::simplify(intersections);
}

std::vector<char_range> char_range::group_substract(std::vector<char_range> first, const std::vector<char_range>& second) {
    for(const char_range& s : second) {
        std::vector<char_range> new_result;
        for(const char_range& f : first) {
            std::vector<char_range> remaining = f.substract(s);
            new_result.insert(new_result.end(), remaining.begin(), remaining.end());
        }
        first = new_result;
    }

    return char_range::simplify(first);
}



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

const std::vector<branch*>& sequence_branch::get_elements() {
    return this->elements;
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

branch* quantifier_branch::get_child() {
    return this->child;
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
    std::string as_str = "Character_Set{neg: " + std::to_string(this->negated) + ", chars: ";

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