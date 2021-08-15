#include <iostream>
#include "expression.h"

Expression::Expression(BinaryExprOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void Expression::accept(Visitor* v)
{
    v->visit(this);
}

void Expression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "EXPRESSION" << std::endl;
    binary_op->print(tabs + 1);
}
