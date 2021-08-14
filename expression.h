#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "visitor.h"
#include "binaryop.h"


class Expression
{
public:
    Expression(BinaryExprOp* binary_op);
    void accept(Visitor* v);

    BinaryExprOp* binary_op;
};

#endif // EXPRESSION_H
