#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "literal.h"


class Expression
{
public:
    Expression(Literal* literal);

private:
    Literal* value;
};

#endif // EXPRESSION_H
