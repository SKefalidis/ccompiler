#include <iostream>
#include "additiveexpression.h"
#include "term.h"

AdditiveExpression::AdditiveExpression(Term* term)
    : term(term)
{
    ;
}

AdditiveExpression::AdditiveExpression(Term* term, Token op, AdditiveExpression* tail)
    : term(term), op(op), tail(tail)
{
    ;
}

void AdditiveExpression::accept(Visitor* v)
{
    v->visit(this);
}

void AdditiveExpression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "ADDITIVE EXPRESSION" << std::endl;
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (tail) {
        tail->print_node(tabs);
    }
    term->print_node(tabs);
}
