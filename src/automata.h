#pragma once

#include <vector>
#include <cstddef>
#include <string>
#include <utility>
#include <set>
#include <map>

#include "regex/ast.h"

namespace automata {
    struct connection {
        size_t node_id;
        std::vector<ast::char_range> requirements;
        bool negated = false;

        bool is_epsilon_connection() const;
        std::vector<ast::char_range> resolve_negation() const;

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
            static void add_dfa_connections(const std::vector<std::set<size_t>>& enclosures, automaton& dfa, const automaton& nfa,
                                     std::map<std::set<size_t>, size_t>& node_ids, const std::set<size_t>& curr);
            static size_t create_dfa_node(const std::set<size_t>& nodes, const automaton& nfa, automaton& dfa, std::map<std::set<size_t>, size_t>& node_ids);

            std::vector<std::set<size_t>> compute_epsilon_enclosures();
            std::set<size_t> compute_epsilon_enclosure(const size_t node, const std::set<size_t>& visited = std::set<size_t>{});
        public:
            automaton();
            bool add_connection(const size_t node_from, const size_t node_to, const std::vector<ast::char_range>& requirements = {}, const bool negated = false);
            size_t add_node(const std::string& root_tag = "");
            const std::vector<node> get_nodes() const;
            std::string to_string();
            ~automaton();

            static automaton nfa_from_token_rules(const std::vector<std::pair<std::string, ast::branch*>>& rules);
            static automaton dfa_from_nfa(automaton nfa);
    };
}