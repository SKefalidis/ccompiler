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
