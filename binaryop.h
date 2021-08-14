#ifndef BINARYOP_H
#define BINARYOP_H

#include "token.h"

class Factor;
class Term;


class BinaryFactorOp
{
public:
    BinaryFactorOp(Token op, BinaryFactorOp* fact, Factor* next_fact);

    BinaryFactorOp* fact;
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
