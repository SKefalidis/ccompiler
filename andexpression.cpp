#include <iostream>
#include "andexpression.h"

AndExpression::AndExpression(BinaryAndExprOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void AndExpression::accept(Visitor* v)
{
    v->visit(this);
}

void AndExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "AND EXPRESSION" << std::endl;
    binary_op->print(tabs + 1);
}
