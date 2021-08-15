#ifndef RELATIONALEXPRESSION_H
#define RELATIONALEXPRESSION_H

#include "node.h"
#include "visitor.h"
#include "binaryop.h"

class RelationalExpression : public Node
{
public:
    RelationalExpression(BinaryRelExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryRelExprOp* binary_op;
};

#endif // RELATIONALEXPRESSION_H
