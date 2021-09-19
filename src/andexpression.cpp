#include <iostream>
#include "andexpression.h"
#include "equalityexpression.h"

AndExpression::AndExpression(EqualityExpression* expr)
    : expr(expr), Node(NodeType::AND_EXPRESSION)
{
    ;
}

AndExpression::AndExpression(EqualityExpression* expr, Token op, AndExpression* tail)
    : expr(expr), op(op), tail(tail), Node(NodeType::AND_EXPRESSION)
{
    ;
}

void AndExpression::accept(Visitor* v)
{
    v->visit(this);
}

void AndExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "AND EXPRESSION" << std::endl;
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (tail) {
        tail->print_node(tabs);
    }
    expr->print_node(tabs);
}
