#include "term.h"

Term::Term(BinaryFactorOp* binary_op)
    : binary_op(binary_op)
{
    ;
}

void Term::accept(Visitor* v)
{
    v->visit(this);
}
