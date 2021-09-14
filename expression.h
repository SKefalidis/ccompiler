#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "node.h"
#include "orexpression.h"
#include "visitor.h"


class Expression : public Node
{
public:
    Expression(CondExpression* cond_expr);
    Expression(std::string id, Expression* expr, Token op);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    CondExpression* cond_expr   { nullptr };
    Expression*     expr        { nullptr };
    std::string     id          { "" };
    Token           op;
};

#endif // EXPRESSION_H
