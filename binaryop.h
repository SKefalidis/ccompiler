#ifndef BINARYOP_H
#define BINARYOP_H

#include "token.h"

class Factor;
class Term;


class BinaryTermOp
{
public:
    BinaryTermOp(Token op, BinaryTermOp* fact, Factor* next_fact);

    BinaryTermOp* fact;
    Factor* next_fact;
    Token op;
};

class BinaryExprOp
{
public:
    BinaryExprOp(Token op, BinaryExprOp* term, Term* next_term);

    BinaryExprOp* term;
    Term* next_term;
    Token op;
};

#endif // BINARYOP_H
