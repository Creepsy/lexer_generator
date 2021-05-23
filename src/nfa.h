#pragma once

#include <vector>
#include <cstddef>

#include "regex/ast.h"

namespace automata {
    struct connection {
        size_t node_id;
        std::vector<ast::char_range> requirements;

        bool is_epsilon_connection();
    };

    struct node {
        std::vector<connection> connections;
    };

    class nfa {
        private:
            std::vector<node> nodes;
        public:
            nfa();
            ~nfa();

            static nfa from_ast(ast::branch* ast);
    };
}