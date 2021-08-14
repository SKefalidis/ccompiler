#ifndef TERM_H
#define TERM_H

#include "visitor.h"
#include "binaryop.h"


class Term
{
public:
    Term(BinaryFactorOp* binary_op);
    void accept(Visitor* v);

    BinaryFactorOp* binary_op;
};

#endif // TERM_H
