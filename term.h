#ifndef TERM_H
#define TERM_H

#include "node.h"
#include "visitor.h"
#include "token.h"


class Term : public Node
{
public:
    Term(Factor* fact);
    Term(Factor* fact, Token op, Term* tail);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Factor* fact;
    Token op;
    Term* tail;
};

#endif // TERM_H
