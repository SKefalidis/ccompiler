#include <iostream>
#include "term.h"

Term::Term(BinaryTermOp* binary_op)
    : binary_op(binary_op)
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
    binary_op->print(tabs + 1);
}
