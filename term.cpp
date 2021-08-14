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
