#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stack>
#include <string>
#include "token.h"

#include "goal.h"
#include "expression.h"
#include "function.h"
#include "literal.h"
#include "return.h"
#include "statement.h"


class Parser
{
public:
    Parser(std::vector<Token> tokens);
    Goal* parse();

private:
    Token peek() const;
    Token consume();
    Token consume_and_check(TokenType expected);
    void parse_error(std::string error);

    Expression* exp();
    Function* func();
    Statement* stm();

    std::vector<Token> tokens;
    int current_token;

    std::stack<void*> nodes;
};

#endif // PARSER_H
