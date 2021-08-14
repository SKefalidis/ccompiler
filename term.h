#ifndef TERM_H
#define TERM_H

#include "node.h"
#include "visitor.h"
#include "binaryop.h"


class Term : public Node
{
public:
    Term(BinaryTermOp* binary_op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    BinaryTermOp* binary_op;
};

#endif // TERM_H
