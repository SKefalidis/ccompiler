#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "visitor.h"
#include "literal.h"


class Expression
{
public:
    Expression(Literal* literal);
    void accept(Visitor* v);

    Literal* value;
};

#endif // EXPRESSION_H
