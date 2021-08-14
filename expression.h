#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "binaryop.h"


class Expression : public Node
{
public:
    Expression(BinaryExprOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryExprOp* binary_op;
};

#endif // EXPRESSION_H
