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
    if (term()) {
        BinaryExprOp* b = new BinaryExprOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<Term*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::PLUS || t.type == TokenType::MINUS) {
            Token op = consume();
            Term* next_term = term();
            b = new BinaryExprOp(op, b, next_term);
            t = peek();
        }

        e = new Expression(b);
        nodes.push(e);
    } else {
        return NULL;
    }
    return e;
}

Factor* Parser::fact()
{
    Factor* f;
    if (peek().type == TokenType::INTEGER_LITERAL) {
        Token t = consume_and_check(TokenType::INTEGER_LITERAL);
        if (t.type == TokenType::INVALID) {
            return NULL;
        }

        f = new Factor(new IntLiteral(t.value));
        nodes.push(f);
    } else if (peek().type == TokenType::LPAREN) {
        consume_and_check(TokenType::LPAREN);
        Expression* e = exp();
        nodes.pop(); /* not needing the stack */
        consume_and_check(TokenType::RPAREN);

        f = new Factor(e);
        nodes.push(f);
    } else if (unary_op() && fact()) {
        Factor* inner_factor = static_cast<Factor*>(nodes.top());
        nodes.pop();
        UnaryOperator* op = static_cast<UnaryOperator*>(nodes.top());
        nodes.pop();

        f = new Factor(op, inner_factor);
        nodes.push(f);
    } else {
        return NULL;
    }
    return f;
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

Term* Parser::term()
{
    Term* term;
    if (fact()) {
        BinaryFactorOp* b = new BinaryFactorOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<Factor*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::STAR || t.type == TokenType::SLASH) {
            Token op = consume();
            Factor* next_fact = fact();
            b = new BinaryFactorOp(op, b, next_fact);
            t = peek();
        }

        term = new Term(b);
        nodes.push(term);
    } else {
        return NULL;
    }
    return term;
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
