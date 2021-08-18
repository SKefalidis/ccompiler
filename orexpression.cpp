#include <iostream>
#include "orexpression.h"
#include "andexpression.h"


OrExpression::OrExpression(AndExpression* expr)
    : expr(expr)
{
    ;
}

OrExpression::OrExpression(AndExpression* expr, Token op, OrExpression* tail)
    : expr(expr), op(op), tail(tail)
{
    ;
}

void OrExpression::accept(Visitor* v)
{
    v->visit(this);
}

void OrExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "OR EXPRESSION" << std::endl;
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (tail) {
        tail->print_node(tabs);
    }
    expr->print_node(tabs);
}
