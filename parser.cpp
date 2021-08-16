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

AdditiveExpression* Parser::add_expr()
{
    AdditiveExpression* e;
    if (term()) {
        BinaryAddExprOp* b = new BinaryAddExprOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<Term*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::PLUS || t.type == TokenType::MINUS) {
            Token op = consume();
            Term* next_term = term();
            nodes.pop();
            b = new BinaryAddExprOp(op, b, next_term);
            t = peek();
        }

        e = new AdditiveExpression(b);
        nodes.push(e);
    } else {
        return NULL;
    }
    return e;
}

AndExpression* Parser::and_expr()
{
    AndExpression* e;
    if (eq_expr()) {
        BinaryAndExprOp* b = new BinaryAndExprOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<EqualityExpression*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::AND) {
            Token op = consume();
            EqualityExpression* next_expr = eq_expr();
            nodes.pop();
            b = new BinaryAndExprOp(op, b, next_expr);
            t = peek();
        }

        e = new AndExpression(b);
        nodes.push(e);
    } else {
        return NULL;
    }
    return e;
}

EqualityExpression* Parser::eq_expr()
{
    EqualityExpression* e;
    if (rel_expr()) {
        BinaryEqExprOp* b = new BinaryEqExprOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<RelationalExpression*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::EQ || t.type == TokenType::NEQ) {
            Token op = consume();
            RelationalExpression* next_expr = rel_expr();
            nodes.pop();
            b = new BinaryEqExprOp(op, b, next_expr);
            t = peek();
        }

        e = new EqualityExpression(b);
        nodes.push(e);
    } else {
        return NULL;
    }
    return e;
}

Expression* Parser::expr()
{
    Expression* e;
    if (and_expr()) {
        BinaryExprOp* b = new BinaryExprOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<AndExpression*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::OR) {
            Token op = consume();
            AndExpression* next_expr = and_expr();
            nodes.pop();
            b = new BinaryExprOp(op, b, next_expr);
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
    } else if (peek().type == TokenType::LPAREN) {
        consume_and_check(TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop(); /* not needing the stack */
        consume_and_check(TokenType::RPAREN);

        f = new Factor(e);
    } else if (peek().type == TokenType::IDENTIFIER) {
        std::string variable = consume().value;

        f = new Factor(variable);
    } else if (unary_op() && fact()) {
        Factor* inner_factor = static_cast<Factor*>(nodes.top());
        nodes.pop();
        UnaryOperator* op = static_cast<UnaryOperator*>(nodes.top());
        nodes.pop();

        f = new Factor(op, inner_factor);
    } else {
        return NULL;
    }
    nodes.push(f);
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

RelationalExpression* Parser::rel_expr()
{
    RelationalExpression* e;
    if (add_expr()) {
        BinaryRelExprOp* b = new BinaryRelExprOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<AdditiveExpression*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::LT || t.type == TokenType::LE || t.type == TokenType::GT || t.type == TokenType::GE) {
            Token op = consume();
            AdditiveExpression* next_expr = add_expr();
            nodes.pop();
            b = new BinaryRelExprOp(op, b, next_expr);
            t = peek();
        }

        e = new RelationalExpression(b);
        nodes.push(e);
    } else {
        return NULL;
    }
    return e;
}

Statement* Parser::stm()
{
    Statement* s;
    consume_and_check(TokenType::RETURN);
    if (expr()) {
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
        BinaryTermOp* b = new BinaryTermOp(Token(TokenType::INVALID, "INVALID"), NULL, static_cast<Factor*>(nodes.top()));
        nodes.pop();

        Token t = peek();
        while (t.type == TokenType::STAR || t.type == TokenType::SLASH) {
            Token op = consume();
            Factor* next_fact = fact();
            nodes.pop();
            b = new BinaryTermOp(op, b, next_fact);
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
