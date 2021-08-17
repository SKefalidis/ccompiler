#include <iostream>
#include "orexpression.h"

OrExpression::OrExpression(BinaryOrExprOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void OrExpression::accept(Visitor* v)
{
    v->visit(this);
}

void OrExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "OR EXPRESSION" << std::endl;
    binary_op->print(tabs + 1);
}
