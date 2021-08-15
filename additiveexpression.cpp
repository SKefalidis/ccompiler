#include <iostream>
#include "additiveexpression.h"

AdditiveExpression::AdditiveExpression(BinaryAddExprOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void AdditiveExpression::accept(Visitor* v)
{
    v->visit(this);
}

void AdditiveExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "ADDITIVE EXPRESSION" << std::endl;
    binary_op->print(tabs + 1);
}
