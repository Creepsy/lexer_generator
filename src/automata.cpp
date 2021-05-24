#include "automata.h"

using namespace automata;

bool connection::is_epsilon_connection() {
    return this->requirements.empty();
}

std::string connection::to_string() {
    std::string as_str = "(->" + std::to_string(this->node_id) + ", neg: " + std::to_string(this->negated) + ", ";

    if(this->is_epsilon_connection()) {
        as_str += "e_conn";
    } else {
        as_str += "{";
        for(const ast::char_range c : this->requirements) {
            if(c.negated) as_str += "!";
            if(c.start == c.end) {
                as_str += c.start;
            } else {
                as_str += "[" + std::string(1, c.start) + "-" + std::string(1, c.end) + "]";
            }
        }

        as_str += "}";
    }

    return as_str + ")";
}



automaton::automaton() : nodes{} {
}

bool automaton::add_connection(const size_t node_from, const size_t node_to, const std::vector<ast::char_range>& requirements, const bool negated) {
    if(this->nodes.size() <= node_from || this->nodes.size() <= node_to) return false;

    this->nodes[node_from].connections.push_back(connection{node_to, requirements, negated});

    return true;
}

size_t automaton::add_node(const std::string& root_tag) {
    this->nodes.push_back(node{std::vector<connection>{}, root_tag});
    return this->nodes.size() - 1;
}

std::string automaton::to_string() {
    std::string as_str = "{";
    for(size_t n = 0; n < this->nodes.size(); n++) {
        if(n != 0) as_str += ", ";

        as_str += "Node(" + std::to_string(n) + "): ";
        if(this->nodes[n].root_tag != "") as_str += this->nodes[n].root_tag + "; ";

        as_str += "{";

        for(size_t c = 0; c < this->nodes[n].connections.size(); c++) {
            if(c != 0) as_str += ", ";
            as_str += this->nodes[n].connections[c].to_string();
        }

        as_str += "}";
    }

    return as_str + "}";
}

automaton::~automaton() {
}

// static

automaton automaton::from_token_rules(const std::vector<std::pair<std::string, ast::branch*>>& rules) {
    automaton machine{};
    size_t origin_node = machine.add_node();

    for(const std::pair<std::string, ast::branch*>& rule : rules) {
        size_t branch_node = machine.add_node();
        machine.add_connection(origin_node, branch_node);
        
        size_t end_node = automaton::insert_branch(machine, rule.second, branch_node);
        
        size_t root_node = machine.add_node(rule.first);
        machine.add_connection(end_node, root_node);
    }

    return machine;
}

// private, static

size_t automaton::insert_branch(automaton& machine, ast::branch* b, const size_t origin_node) {
    if(dynamic_cast<ast::quantifier_branch*>(b)) {
        ast::quantifier_branch* quantifier = dynamic_cast<ast::quantifier_branch*>(b);
        return automaton::insert_quantifier(machine, quantifier, origin_node);
    } else if(dynamic_cast<ast::sequence_branch*>(b)) {
        ast::sequence_branch* sequence = dynamic_cast<ast::sequence_branch*>(b);
        if(sequence->is_option()) {
            return automaton::insert_option(machine, sequence, origin_node);
        } else {
            return automaton::insert_sequence(machine, sequence, origin_node);
        }
    } else {
        ast::character_set_branch* character = dynamic_cast<ast::character_set_branch*>(b);
        return automaton::insert_character(machine, character, origin_node);
    }
}

size_t automaton::insert_option(automaton& machine, ast::sequence_branch* b, const size_t origin_node) {
    if(!b->is_option()) return origin_node;

    std::vector<size_t> roots;

    for(ast::branch* option : b->get_elements()) {
        size_t branch_node = machine.add_node();
        machine.add_connection(origin_node, branch_node);
        
        roots.push_back(automaton::insert_branch(machine, option, branch_node));
    }

    if(roots.size() > 1) {
        size_t group_node = machine.add_node();
        for(const size_t root : roots) {
            machine.add_connection(root, group_node);
        }
        return group_node;
    }

    return roots[0];
}

size_t automaton::insert_sequence(automaton& machine, ast::sequence_branch* b, const size_t origin_node) {
    if(b->is_option()) return origin_node;

    size_t prev_node = origin_node;

    for(ast::branch* element : b->get_elements()) {
        prev_node = automaton::insert_branch(machine, element, prev_node);
    }

    return prev_node;
}

size_t automaton::insert_quantifier(automaton& machine, ast::quantifier_branch* b, const size_t origin_node) {
    size_t curr_node = machine.add_node();
    machine.add_connection(origin_node, curr_node);

    size_t previous_node = curr_node;

    for(size_t c = 0; c < b->get_minimum(); c++) {
        previous_node = curr_node;
        curr_node = automaton::insert_branch(machine, b->get_child(), curr_node);
    }

    if(b->is_limited()) {
        std::vector<size_t> to_connect{};

        for(size_t c = 0; c < b->get_maximum() - b->get_minimum(); c++) {
            to_connect.push_back(curr_node);
            curr_node = automaton::insert_branch(machine, b->get_child(), curr_node);
        }

        for(const size_t n : to_connect) {
            machine.add_connection(n, curr_node);
        }
    } else {
        machine.add_connection(curr_node, previous_node);
    }

    return curr_node;
}

size_t automaton::insert_character(automaton& machine, ast::character_set_branch* b, const size_t origin_node) {

    size_t end_node = machine.add_node();
    machine.add_connection(origin_node, end_node, b->get_characters(), b->is_negated());

    return end_node;
}