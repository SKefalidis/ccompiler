#ifndef OREXPRESSION_H
#define OREXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "binaryop.h"

class OrExpression : public Node
{
public:
    OrExpression(BinaryOrExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryOrExprOp* binary_op;
};

#endif // OREXPRESSION_H
