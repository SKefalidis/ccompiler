#include "unaryoperator.h"

UnaryOperator::UnaryOperator(Token t) : op(t)
{
    ;
}

void UnaryOperator::accept(Visitor* v)
{
    v->visit(this);
}
