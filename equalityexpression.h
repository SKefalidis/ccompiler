#ifndef EQUALITYEXPRESSION_H
#define EQUALITYEXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "binaryop.h"

class EqualityExpression : public Node
{
public:
    EqualityExpression(BinaryEqExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryEqExprOp* binary_op;
};

#endif // EQUALITYEXPRESSION_H
