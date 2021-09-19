#include <iostream>
#include "term.h"
#include "factor.h"


Term::Term(Factor* fact)
    : fact(fact), Node(NodeType::TERM)
{
    ;
}

Term::Term(Factor* fact, Token op, Term* tail)
    : fact(fact), op(op), tail(tail), Node(NodeType::TERM)
{
    ;
}

void Term::accept(Visitor* v)
{
    v->visit(this);
}

void Term::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "TERM" << std::endl;
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (tail) {
        tail->print_node(tabs);
    }
    fact->print_node(tabs);
}
