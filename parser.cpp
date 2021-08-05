#include <iostream>
#include "parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens)
{
    ;
}

Token Parser::peek() const
{
    return tokens.at(current_token);
}

Token Parser::consume()
{
    return tokens.at(current_token++);
}

Token Parser::consume_and_check(TokenType expected)
{
    Token t = consume();
    if (t.type != expected) {
        parse_error(tokenTypeStrings.at(expected));
        return Token(TokenType::INVALID, "invalid");
    }
    return t;
}

void Parser::parse_error(std::string error)
{
    std::cerr << "Expected '" + error + "'" << std::endl;
    throw "Expected '" + error + "'";
}

Goal* Parser::parse()
{
    Goal* g;
    current_token = 0;
    if (func() && peek().type == TokenType::END_OF_FILE) {
        Function* f = static_cast<Function*>(nodes.top());
        g = new Goal(f);
        nodes.pop();
        return g;
    } else {
        return NULL;
    }
}

Expression* Parser::exp()
{
    Expression* e;
    if (peek().type == TokenType::INTEGER_LITERAL) {
        Token t = consume_and_check(TokenType::INTEGER_LITERAL);
        if (t.type == TokenType::INVALID) {
            return NULL;
        }

        e = new Expression(new IntLiteral(t.value));
        nodes.push(e);
    } else if (unary_op() && exp()) {
        Expression* inner_expr = static_cast<Expression*>(nodes.top());
        nodes.pop();
        UnaryOperator* op = static_cast<UnaryOperator*>(nodes.top());
        nodes.pop();

        e = new Expression(op, inner_expr);
        nodes.push(e);
    } else {
        return NULL;
    }
    return e;
}

Function* Parser::func()
{
    Function* f;
    consume_and_check(TokenType::INT);
    std::string name = consume_and_check(TokenType::IDENTIFIER).value;
    consume_and_check(TokenType::LPAREN);
    consume_and_check(TokenType::RPAREN);
    consume_and_check(TokenType::LBRACE);
    if (stm()) {
        Statement* s = static_cast<Statement*>(nodes.top());
        f = new Function(name, s);
        nodes.pop();
        nodes.push(f);
    } else {
        return NULL;
    }
    consume_and_check(TokenType::RBRACE);
    return f;
}

Statement* Parser::stm()
{
    Statement* s;
    consume_and_check(TokenType::RETURN);
    if (exp()) {
        Expression* e = static_cast<Expression*>(nodes.top());
        s = new Statement(e);
        nodes.pop();
        nodes.push(s);
    } else {
        return NULL;
    }
    consume_and_check(TokenType::SEMICOLON);
    return s;
}

UnaryOperator* Parser::unary_op()
{
    UnaryOperator* op;
    auto type = peek().type;
    if (type == TokenType::COMPLEMENT || type == TokenType::MINUS || type == TokenType::NEGATION) {
        Token t = consume();
        op = new UnaryOperator(t);
        nodes.push(op);
    } else {
        parse_error("Unary Operator");
        return NULL;
    }
    return op;
}
