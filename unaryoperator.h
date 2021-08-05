#ifndef UNARYOPERATOR_H
#define UNARYOPERATOR_H

#include "visitor.h"
#include "token.h"


class UnaryOperator
{
public:
    UnaryOperator(Token t);
    void accept(Visitor* v);

    Token op;
};

#endif // UNARYOPERATOR_H
