#ifndef ANDEXPRESSION_H
#define ANDEXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "binaryop.h"


class AndExpression : public Node
{
public:
    AndExpression(BinaryAndExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryAndExprOp* binary_op;
};

#endif // ANDEXPRESSION_H
