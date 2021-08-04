#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"


class Statement
{
public:
    Statement(Expression* expr);

private:
    Expression* expr;
};

#endif // STATEMENT_H
