#ifndef ADDITIVEEXPRESSION_H
#define ADDITIVEEXPRESSION_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "binaryop.h"


class AdditiveExpression : public Node
{
public:
    AdditiveExpression(BinaryAddExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryAddExprOp* binary_op;
};

#endif // ADDITIVEEXPRESSION_H
