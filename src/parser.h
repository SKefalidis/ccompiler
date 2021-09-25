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
#include "expressionoptional.h"
#include "orexpression.h"
#include "factor.h"
#include "functiondeclaration.h"
#include "functioncall.h"
#include "literal.h"
#include "relationalexpression.h"
#include "statement.h"
#include "term.h"
#include "type.h"
#include "unaryoperator.h"


class Parser
{
public:
    Parser(std::vector<Token> tokens);
    Goal* parse();

private:
    Token peek(int offset = 0) const;
    bool expect(TokenType expected, int offset = 0);
    bool expect(std::vector<TokenType> expected, int offset = 0);
    Token consume();
    void backtrack();
    void backtrack(int i);

    void parse_error();

    // TODO: make stack optional!
    AdditiveExpression*     add_expr();
    AndExpression*          and_expr();
    BlockItem*              block_item();
    CondExpression*         cond_expr();
    Declaration*            decl();
    EqualityExpression*     eq_expr();
    Expression*             expr();
    ExpressionOptional*     expr_optional();
    OrExpression*           or_expr();
    Factor*                 fact();
    FunctionDeclaration*    func_decl();
    FunctionCall*           func_call();
    RelationalExpression*   rel_expr();
    Statement*              stm();
    Term*                   term();
    Type*                   type();
    UnaryOperator*          unary_op();

    std::vector<Token> tokens   {};
    int current_token           { 0 };
    std::stack<int> tokens_used {};

    /* There are 2 kinds of error logged, peeking and consumption errors.
     * Consumption errors have a negative value and are prioritized.
     */
    std::vector<std::pair<std::string, int>> errors {}; /* error_msg, tokens consumed */
};

#endif // PARSER_H
