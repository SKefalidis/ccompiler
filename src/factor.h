#ifndef FACTOR_H
#define FACTOR_H

#include <string>
#include "visitor.h"
#include "node.h"
#include "literal.h"
#include "unaryoperator.h"
#include "functioncall.h"


class Factor : public Node
{
public:
    Factor(Literal* literal);
    Factor(UnaryOperator* op, Factor* factor);
    Factor(Expression* expr);
    Factor(std::string variable);
    Factor(FunctionCall *func);

    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Literal* value          { nullptr };
    UnaryOperator* unary_op { nullptr };
    Factor* inner_factor    { nullptr };
    Expression* expr        { nullptr };
    FunctionCall *func      { nullptr };
    std::string variable    { "" };
};

#endif // FACTOR_H
