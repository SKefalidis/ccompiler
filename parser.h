#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include <string>
#include "token.h"

#include "binaryop.h"
#include "goal.h"
#include "additiveexpression.h"
#include "andexpression.h"
#include "equalityexpression.h"
#include "expression.h"
#include "orexpression.h"
#include "factor.h"
#include "function.h"
#include "literal.h"
#include "relationalexpression.h"
#include "return.h"
#include "statement.h"
#include "term.h"
#include "unaryoperator.h"


class Parser
{
public:
    Parser(std::vector<Token> tokens);
    Goal* parse();

private:
    Token peek(int offset = 0) const;
    Token consume();
    Token consume_and_check(TokenType expected);

    void* get_and_pop();

    void parse_error(std::string error);

    AdditiveExpression*     add_expr();
    AndExpression*          and_expr();
    EqualityExpression*     eq_expr();
    Expression*             expr();
    OrExpression*           or_expr();
    Factor*                 fact();
    Function*               func();
    RelationalExpression*   rel_expr();
    Statement*              stm();
    Term*                   term();
    UnaryOperator*          unary_op();

    std::vector<Token> tokens;
    int current_token           { 0 };

    std::stack<void*> nodes     {};
};

#endif // PARSER_H
