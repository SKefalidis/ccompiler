#include <iostream>
#include "relationalexpression.h"
#include "additiveexpression.h"

RelationalExpression::RelationalExpression(AdditiveExpression* expr)
    : expr(expr), Node(NodeType::RELATIONAL_EXPRESSION)
{
    ;
}

RelationalExpression::RelationalExpression(AdditiveExpression* expr, Token op, RelationalExpression* tail)
    : expr(expr), op(op), tail(tail), Node(NodeType::RELATIONAL_EXPRESSION)
{
    ;
}

void RelationalExpression::accept(Visitor* v)
{
    v->visit(this);
}

void RelationalExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "RELATIONAL EXPRESSION" << std::endl;
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (tail) {
        tail->print_node(tabs);
    }
    expr->print_node(tabs);
}
