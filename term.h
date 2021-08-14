#ifndef TERM_H
#define TERM_H

#include "visitor.h"
#include "binaryop.h"


class Term
{
public:
    Term(BinaryTermOp* binary_op);
    void accept(Visitor* v);

    BinaryTermOp* binary_op;
};

#endif // TERM_H
