#include <iostream>
#include "additiveexpression.h"
#include "andexpression.h"
#include "binaryop.h"
#include "equalityexpression.h"
#include "relationalexpression.h"
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

BinaryExprOp::BinaryExprOp(Token op, BinaryExprOp* expr, AndExpression* next_expr)
    : op(op), expr(expr), next_expr(next_expr)
{
    ;
}

void BinaryExprOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (expr) {
        expr->print(tabs);
    }
    next_expr->print_node(tabs);
}

BinaryAddExprOp::BinaryAddExprOp(Token op, BinaryAddExprOp* term, Term* next_term)
    : op(op), term(term), next_term(next_term)
{
    ;
}

void BinaryAddExprOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (term) {
        term->print(tabs);
    }
    next_term->print_node(tabs);
}

BinaryAndExprOp::BinaryAndExprOp(Token op, BinaryAndExprOp* expr, EqualityExpression* next_expr)
    : op(op), expr(expr), next_expr(next_expr)
{
    ;
}

void BinaryAndExprOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (expr) {
        expr->print(tabs);
    }
    next_expr->print_node(tabs);
}

BinaryEqExprOp::BinaryEqExprOp(Token op, BinaryEqExprOp* expr, RelationalExpression* next_expr)
    : op(op), expr(expr), next_expr(next_expr)
{
    ;
}

void BinaryEqExprOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (expr) {
        expr->print(tabs);
    }
    next_expr->print_node(tabs);
}


BinaryRelExprOp::BinaryRelExprOp(Token op, BinaryRelExprOp* expr, AdditiveExpression* next_expr)
    : op(op), expr(expr), next_expr(next_expr)
{
    ;
}

void BinaryRelExprOp::print(int tabs)
{
    if (op.type != TokenType::INVALID) {
        std::cout << Node::tabs_string(tabs) << tokenTypeStrings.at(op.type) << std::endl;
    }
    if (expr) {
        expr->print(tabs);
    }
    next_expr->print_node(tabs);
}
