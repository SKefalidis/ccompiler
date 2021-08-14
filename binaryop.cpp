#include "binaryop.h"

BinaryTermOp::BinaryTermOp(Token op, BinaryTermOp* fact, Factor* next_fact)
    : op(op), fact(fact), next_fact(next_fact)
{
    ;
}

BinaryExprOp::BinaryExprOp(Token op, BinaryExprOp* term, Term* next_term)
    : op(op), term(term), next_term(next_term)
{
    ;
}
