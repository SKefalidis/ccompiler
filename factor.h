#ifndef FACTOR_H
#define FACTOR_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "orexpression.h"
#include "literal.h"
#include "unaryoperator.h"


class Factor : public Node
{
public:
    Factor(Literal* literal);
    Factor(UnaryOperator* op, Factor* factor);
    Factor(OrExpression* expr);
    Factor(std::string variable);

    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Literal* value          { nullptr };
    UnaryOperator* unary_op { nullptr };
    Factor* inner_factor    { nullptr };
    OrExpression* expr      { nullptr };
    std::string variable    { "" };
};

#endif // FACTOR_H
