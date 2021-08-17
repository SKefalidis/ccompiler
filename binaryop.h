#ifndef BINARYOP_H
#define BINARYOP_H

#include "token.h"

class AdditiveExpression;
class AndExpression;
class EqualityExpression;
class RelationalExpression;
class Factor;
class Term;


class BinaryTermOp
{
public:
    BinaryTermOp(Token op, BinaryTermOp* fact, Factor* next_fact);
    void print(int tabs);

    BinaryTermOp* fact;
    Factor* next_fact;
    Token op;
};

class BinaryOrExprOp
{
public:
    BinaryOrExprOp(Token op, BinaryOrExprOp* expr, AndExpression* next_expr);
    void print(int tabs);

    BinaryOrExprOp* expr;
    AndExpression* next_expr;
    Token op;
};

class BinaryAddExprOp
{
public:
    BinaryAddExprOp(Token op, BinaryAddExprOp* term, Term* next_term);
    void print(int tabs);

    BinaryAddExprOp* term;
    Term* next_term;
    Token op;
};

class BinaryAndExprOp
{
public:
    BinaryAndExprOp(Token op, BinaryAndExprOp* expr, EqualityExpression* next_expr);
    void print(int tabs);

    BinaryAndExprOp* expr;
    EqualityExpression* next_expr;
    Token op;
};

class BinaryEqExprOp
{
public:
    BinaryEqExprOp(Token op, BinaryEqExprOp* expr, RelationalExpression* next_expr);
    void print(int tabs);

    BinaryEqExprOp* expr;
    RelationalExpression* next_expr;
    Token op;
};

class BinaryRelExprOp
{
public:
    BinaryRelExprOp(Token op, BinaryRelExprOp* expr, AdditiveExpression* next_expr);
    void print(int tabs);

    BinaryRelExprOp* expr;
    AdditiveExpression* next_expr;
    Token op;
};

#endif // BINARYOP_H
