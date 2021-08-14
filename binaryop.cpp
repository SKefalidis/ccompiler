#include <iostream>
#include "binaryop.h"
#include "factor.h"
#include "term.h"

BinaryTermOp::BinaryTermOp(Token op, BinaryTermOp* fact, Factor* next_fact)
    : op(op), fact(fact), next_fact(next_fact)
{
    ;
}

void BinaryTermOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (fact) {
        fact->print(tabs);
    }
    next_fact->print_node(tabs);
}

BinaryExprOp::BinaryExprOp(Token op, BinaryExprOp* term, Term* next_term)
    : op(op), term(term), next_term(next_term)
{
    ;
}

void BinaryExprOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (term) {
        term->print(tabs);
    }
    next_term->print_node(tabs);
}
