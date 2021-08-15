#ifndef FACTOR_H
#define FACTOR_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "expression.h"
#include "literal.h"
#include "unaryoperator.h"


class Factor : public Node
{
public:
    Factor(Literal* literal);
    Factor(UnaryOperator* op, Factor* factor);
    Factor(Expression* expr);

    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Literal* value;
    UnaryOperator* unary_op;
    Factor* inner_factor;
    Expression* expr;
};

#endif // FACTOR_H
