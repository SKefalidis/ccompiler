#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include "visitor.h"


class Statement
{
public:
    Statement(Expression* expr);
    void accept(Visitor* v);

    Expression* expr;
};

#endif // STATEMENT_H
