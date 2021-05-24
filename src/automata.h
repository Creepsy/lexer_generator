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

    class automaton {
        private:
            std::vector<node> nodes;

            static size_t insert_branch(automaton& machine, ast::branch* b, const size_t origin_node);
            static size_t insert_option(automaton& machine, ast::sequence_branch* b, const size_t origin_node);
            static size_t insert_sequence(automaton& machine, ast::sequence_branch* b, const size_t origin_node);
            static size_t insert_quantifier(automaton& machine, ast::quantifier_branch* b, const size_t origin_node);
            static size_t insert_character(automaton& machine, ast::character_set_branch* b, const size_t origin_node);
        public:
            automaton();
            bool add_connection(const size_t node_from, const size_t node_to, const std::vector<ast::char_range>& requirements = {}, const bool negated = false);
            size_t add_node(const std::string& root_tag = "");
            std::string to_string();
            ~automaton();

            static automaton from_token_rules(const std::vector<std::pair<std::string, ast::branch*>>& rules);
    };
}