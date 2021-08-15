#include <iostream>
#include "relationalexpression.h"

RelationalExpression::RelationalExpression(BinaryRelExprOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void RelationalExpression::accept(Visitor* v)
{
    v->visit(this);
}

void RelationalExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "RELATIONAL EXPRESSION" << std::endl;
    binary_op->print(tabs + 1);
}
