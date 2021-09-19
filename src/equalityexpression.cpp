#include <iostream>
#include "equalityexpression.h"
#include "relationalexpression.h"


EqualityExpression::EqualityExpression(RelationalExpression* expr)
    : expr(expr), Node(NodeType::EQUALITY_EXPRESSION)
{
    ;
}

EqualityExpression::EqualityExpression(RelationalExpression* expr, Token op, EqualityExpression* tail)
    : expr(expr), op(op), tail(tail), Node(NodeType::EQUALITY_EXPRESSION)
{
    ;
}

void EqualityExpression::accept(Visitor* v)
{
    v->visit(this);
}

void EqualityExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "EQUALITY EXPRESSION" << std::endl;
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (tail) {
        tail->print_node(tabs);
    }
    expr->print_node(tabs);
}
