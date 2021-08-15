#include <iostream>
#include "equalityexpression.h"


EqualityExpression::EqualityExpression(BinaryEqExprOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void EqualityExpression::accept(Visitor* v)
{
    v->visit(this);
}

void EqualityExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "EQUALITY EXPRESSION" << std::endl;
    binary_op->print(tabs + 1);
}
