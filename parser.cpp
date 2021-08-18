#include <iostream>
#include "parser.h"


Parser::Parser(std::vector<Token> tokens) : tokens(tokens)
{
    ;
}

//---------------------------------------------------------
//   utility functions
//---------------------------------------------------------

Token Parser::peek(int offset) const
{
    return tokens.at(current_token + offset);
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

void* Parser::get_and_pop()
{
    auto x = nodes.top();
    nodes.pop();
    return x;
}

void Parser::parse_error(std::string error)
{
    std::cerr << "Expected '" + error + "'" << std::endl;
    throw "Expected '" + error + "'";
}

//---------------------------------------------------------
//   parsing
//---------------------------------------------------------

Goal* Parser::parse()
{
    Goal* g { nullptr };

    current_token = 0;
    if (func() && peek().type == TokenType::END_OF_FILE) {
        Function* f = static_cast<Function*>(get_and_pop());
        g = new Goal(f);
    }

    return g;
}

AdditiveExpression* Parser::add_expr()
{
    AdditiveExpression* e { nullptr };

    if (term()) {
        BinaryAddExprOp* b = new BinaryAddExprOp(Token(TokenType::INVALID, "INVALID"), nullptr, static_cast<Term*>(get_and_pop()));
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
    }

    return e;
}

AndExpression* Parser::and_expr()
{
    AndExpression* e { nullptr };

    if (eq_expr()) {
        BinaryAndExprOp* b = new BinaryAndExprOp(Token(TokenType::INVALID, "INVALID"), nullptr, static_cast<EqualityExpression*>(get_and_pop()));
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
    }

    return e;
}

EqualityExpression* Parser::eq_expr()
{
    EqualityExpression* e { nullptr };

    if (rel_expr()) {
        BinaryEqExprOp* b = new BinaryEqExprOp(Token(TokenType::INVALID, "INVALID"), nullptr, static_cast<RelationalExpression*>(get_and_pop()));
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
    }

    return e;
}

Expression* Parser::expr()
{
    Expression* e { nullptr };

    if (peek().type == TokenType::IDENTIFIER && peek(1).type == TokenType::ASSIGN) {
        std::string id = consume().value;
        consume_and_check(TokenType::ASSIGN);
        Expression* r_expr = expr();
        nodes.pop();
        e = new Expression(id, r_expr);
        nodes.push(e);
    } else if (or_expr()) {
        OrExpression* o_expr = static_cast<OrExpression*>(get_and_pop());
        e = new Expression(o_expr);
        nodes.push(e);
    }

    return e;
}

OrExpression* Parser::or_expr()
{
    OrExpression* e { nullptr };

    if (and_expr()) {
        BinaryOrExprOp* b = new BinaryOrExprOp(Token(TokenType::INVALID, "INVALID"), nullptr, static_cast<AndExpression*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::OR) {
            Token op = consume();
            AndExpression* next_expr = and_expr();
            nodes.pop();
            b = new BinaryOrExprOp(op, b, next_expr);
            t = peek();
        }

        e = new OrExpression(b);
        nodes.push(e);
    }

    return e;
}

Factor* Parser::fact()
{
    Factor* f { nullptr };

    if (peek().type == TokenType::INTEGER_LITERAL) {
        Token t = consume();
        f = new Factor(new IntLiteral(t.value));
    } else if (peek().type == TokenType::IDENTIFIER) {
        std::string variable = consume().value;
        f = new Factor(variable);
    } else if (peek().type == TokenType::LPAREN) {
        consume();
        OrExpression* e = or_expr();
        nodes.pop();
        consume_and_check(TokenType::RPAREN);
        f = new Factor(e);
    } else if (unary_op() && fact()) {
        Factor* inner_factor = static_cast<Factor*>(get_and_pop());
        UnaryOperator* op = static_cast<UnaryOperator*>(get_and_pop());
        f = new Factor(op, inner_factor);
    } else {
        return nullptr;
    }
    nodes.push(f);

    return f;
}

Function* Parser::func()
{
    Function* f  { nullptr };

    consume_and_check(TokenType::INT);
    std::string name = consume_and_check(TokenType::IDENTIFIER).value;
    consume_and_check(TokenType::LPAREN);
    consume_and_check(TokenType::RPAREN);
    consume_and_check(TokenType::LBRACE);
    std::vector<Statement*> statements {};
    while (peek().type != TokenType::RBRACE) {
        if (stm()) {
            Statement* s = static_cast<Statement*>(get_and_pop());
            statements.push_back(s);
        } else {
            return nullptr;
        }
    }
    f = new Function(name, statements);
    nodes.push(f);
    consume_and_check(TokenType::RBRACE);

    return f;
}

RelationalExpression* Parser::rel_expr()
{
    RelationalExpression* e { nullptr };

    if (add_expr()) {
        BinaryRelExprOp* b = new BinaryRelExprOp(Token(TokenType::INVALID, "INVALID"), nullptr, static_cast<AdditiveExpression*>(get_and_pop()));
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
    }

    return e;
}

Statement* Parser::stm()
{
    Statement* s { nullptr };

    if (peek().type == TokenType::RETURN) {
        consume();
        Expression* e = expr();
        nodes.pop();
        s = new Statement(e, true);
    } else if (peek().type == TokenType::INT) {
        consume();
        std::string id = consume_and_check(TokenType::IDENTIFIER).value;
        Expression* e = nullptr;
        if (peek().type == TokenType::ASSIGN) {
            consume();
            e = expr();
            nodes.pop();
        }

        s = new Statement(e, id);
    } else if (expr()) {
        Expression* e = static_cast<Expression*>(get_and_pop());
        s = new Statement(e, false);
    } else {
        return nullptr;
    }
    nodes.push(s);
    consume_and_check(TokenType::SEMICOLON);

    return s;
}

Term* Parser::term()
{
    Term* term { nullptr };

    if (fact()) {
        BinaryTermOp* b = new BinaryTermOp(Token(TokenType::INVALID, "INVALID"), nullptr, static_cast<Factor*>(get_and_pop()));
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
    }

    return term;
}

UnaryOperator* Parser::unary_op()
{
    UnaryOperator* op { nullptr };

    auto type = peek().type;
    if (type == TokenType::COMPLEMENT || type == TokenType::MINUS || type == TokenType::NEGATION) {
        Token t = consume();
        op = new UnaryOperator(t);
        nodes.push(op);
    } else {
        parse_error("Unary Operator");
        return nullptr;
    }

    return op;
}
