#ifndef ADDITIVEEXPRESSION_H
#define ADDITIVEEXPRESSION_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "binaryop.h"


class AdditiveExpression : public Node
{
public:
    AdditiveExpression(BinaryExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryExprOp* binary_op;
};

#endif // ADDITIVEEXPRESSION_H
