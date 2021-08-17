#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "node.h"
#include "orexpression.h"
#include "visitor.h"


class Expression : public Node
{
public:
    Expression(OrExpression* expr);
    Expression(std::string id, Expression* expr);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    OrExpression*   or_expr { nullptr };
    Expression*     expr    { nullptr };
    std::string     id      { "" };
};

#endif // EXPRESSION_H
