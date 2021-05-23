#include "nfa.h"

using namespace automata;

bool connection::is_epsilon_connection() {
    return this->requirements.empty();
}

nfa::nfa() : nodes{} {
}

nfa::~nfa() {
}

// static
nfa nfa::from_ast(ast::branch* ast) {

}