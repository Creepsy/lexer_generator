#pragma once

#include <vector>
#include <cstddef>
#include <string>
#include <utility>

#include "regex/ast.h"

namespace automata {
    struct connection {
        size_t node_id;
        std::vector<ast::char_range> requirements;
        bool negated = false;

        bool is_epsilon_connection();

        std::string to_string();
    };

    struct node {
        std::vector<connection> connections;
        std::string root_tag = "";
    };

    class nfa {
        private:
            std::vector<node> nodes;

            static size_t insert_branch(nfa& automaton, ast::branch* b, const size_t origin_node);
            static size_t insert_option(nfa& automaton, ast::sequence_branch* b, const size_t origin_node);
            static size_t insert_sequence(nfa& automaton, ast::sequence_branch* b, const size_t origin_node);
            static size_t insert_quantifier(nfa& automaton, ast::quantifier_branch* b, const size_t origin_node);
            static size_t insert_character(nfa& automaton, ast::character_set_branch* b, const size_t origin_node);
        public:
            nfa();
            bool add_connection(const size_t node_from, const size_t node_to, const std::vector<ast::char_range>& requirements = {}, const bool negated = false);
            size_t add_node(const std::string& root_tag = "");
            std::string to_string();
            ~nfa();

            static nfa from_token_rules(const std::vector<std::pair<std::string, ast::branch*>>& rules);
    };
}