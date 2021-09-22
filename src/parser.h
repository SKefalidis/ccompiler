#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include <string>
#include "token.h"

#include "goal.h"
#include "additiveexpression.h"
#include "andexpression.h"
#include "blockitem.h"
#include "condexpression.h"
#include "declaration.h"
#include "equalityexpression.h"
#include "expression.h"
#include "orexpression.h"
#include "factor.h"
#include "functiondeclaration.h"
#include "functioncall.h"
#include "literal.h"
#include "relationalexpression.h"
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
    void backtrack();

    void* get_and_pop();

    void parse_error(std::string error);

    // TODO: make stack optional!
    AdditiveExpression*     add_expr();
    AndExpression*          and_expr();
    BlockItem*              block_item();
    CondExpression*         cond_expr();
    Declaration*            decl();
    EqualityExpression*     eq_expr();
    Expression*             expr();
    Expression*             expr_optional();
    OrExpression*           or_expr();
    Factor*                 fact();
    FunctionDeclaration*    func_decl();
    FunctionCall*           func_call();
    RelationalExpression*   rel_expr();
    Statement*              stm();
    Term*                   term();
    UnaryOperator*          unary_op();

    std::vector<Token> tokens   {};
    int current_token           { 0 };
    std::stack<int> tokens_used {};

    std::stack<Node*> nodes     {};

    std::string error           {};
};

#endif // PARSER_H
