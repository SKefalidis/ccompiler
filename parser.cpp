﻿#include <iostream>
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
        e = new AdditiveExpression(static_cast<Term*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::PLUS || t.type == TokenType::MINUS) {
            Token op = consume();
            Term* next_term = term();
            nodes.pop();
            e = new AdditiveExpression(next_term, op, e);
            t = peek();
        }
        nodes.push(e);
    }

    return e;
}

AndExpression* Parser::and_expr()
{
    AndExpression* e { nullptr };

    if (eq_expr()) {
        e = new AndExpression(static_cast<EqualityExpression*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::AND) {
            Token op = consume();
            EqualityExpression* next_expr = eq_expr();
            nodes.pop();
            e = new AndExpression(next_expr, op, e);
            t = peek();
        }
        nodes.push(e);
    }

    return e;
}

BlockItem* Parser::block_item()
{
    BlockItem* b { nullptr };

    if (peek().type == TokenType::INT && decl()) {
        b = new BlockItem(static_cast<Declaration*>(get_and_pop()));
    } else if (stm()) {
        b = new BlockItem(static_cast<Statement*>(get_and_pop()));
    }
    nodes.push(b);

    return b;
}

CondExpression* Parser::cond_expr()
{
    CondExpression* e { nullptr };

    if (or_expr()) {
        OrExpression* or_expr = static_cast<OrExpression*>(get_and_pop());
        Expression* if_expr { nullptr };
        CondExpression* else_expr { nullptr };
        if (peek().type == TokenType::QUESTION_MARK) {
            consume();
            if_expr = expr();
            consume_and_check(TokenType::COLON);
            else_expr = cond_expr();
        }
        e = new CondExpression(or_expr, if_expr, else_expr);
        nodes.push(e);
    }

    return e;
}

Declaration* Parser::decl()
{
    Declaration* d { nullptr };

    consume_and_check(TokenType::INT);
    std::string id = consume_and_check(TokenType::IDENTIFIER).value;
    Expression* e = nullptr;
    if (peek().type == TokenType::ASSIGN) {
        consume();
        e = expr();
        nodes.pop();
    }
    consume_and_check(TokenType::SEMICOLON);
    d = new Declaration(id, e);
    nodes.push(d);

    return d;
}

EqualityExpression* Parser::eq_expr()
{
    EqualityExpression* e { nullptr };

    if (rel_expr()) {
        e = new EqualityExpression(static_cast<RelationalExpression*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::EQ || t.type == TokenType::NEQ) {
            Token op = consume();
            RelationalExpression* next_expr = rel_expr();
            nodes.pop();
            e = new EqualityExpression(next_expr, op, e);
            t = peek();
        }
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
    } else if (cond_expr()) {
        CondExpression* expr = static_cast<CondExpression*>(get_and_pop());
        e = new Expression(expr);
        nodes.push(e);
    }

    return e;
}

Expression* Parser::expr_optional()
{
    if (peek().type == TokenType::SEMICOLON) {
        return nullptr;
    } else {
        Expression* e = expr();
        if (!e) {
            throw "Crash";
        }
        return e;
    }
}

OrExpression* Parser::or_expr()
{
    OrExpression* e { nullptr };

    if (and_expr()) {
        e = new OrExpression(static_cast<AndExpression*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::OR) {
            Token op = consume();
            AndExpression* next_expr = and_expr();
            nodes.pop();
            e = new OrExpression(next_expr, op, e);
            t = peek();
        }
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
        Expression* e = expr();
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
    std::vector<BlockItem*> items {};
    while (peek().type != TokenType::RBRACE) {
        if (block_item()) {
            BlockItem* b = static_cast<BlockItem*>(get_and_pop()); /* TODO: use `auto` */
            items.push_back(b);
        } else {
            return nullptr;
        }
    }
    f = new Function(name, items);
    nodes.push(f);
    consume_and_check(TokenType::RBRACE);

    return f;
}

RelationalExpression* Parser::rel_expr()
{
    RelationalExpression* e { nullptr };

    if (add_expr()) {
        e = new RelationalExpression(static_cast<AdditiveExpression*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::LT || t.type == TokenType::LE || t.type == TokenType::GT || t.type == TokenType::GE) {
            Token op = consume();
            AdditiveExpression* next_expr = add_expr();
            nodes.pop();
            e = new RelationalExpression(next_expr, op, e);
            t = peek();
        }
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
        consume_and_check(TokenType::SEMICOLON);
    } else if (peek().type == TokenType::BREAK) {
        consume();
        consume_and_check(TokenType::SEMICOLON);
        s = new Statement(Type::BREAK);
    } else if (peek().type == TokenType::CONTINUE) {
        consume();
        consume_and_check(TokenType::SEMICOLON);
        s = new Statement(Type::CONTINUE);
    } else if (peek().type == TokenType::LBRACE) {
        consume();
        std::vector<BlockItem*> block_items;
        while (peek().type != TokenType::RBRACE) {
            if (block_item()) {
                BlockItem* b = static_cast<BlockItem*>(get_and_pop()); /* TODO: use `auto` */
                block_items.push_back(b);
            } else {
                return nullptr;
            }
        }
        consume_and_check(TokenType::RBRACE);
        s = new Statement(block_items);
    } else if (peek().type == TokenType::IF) {
        consume();
        consume_and_check(TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop();
        consume_and_check(TokenType::RPAREN);
        Statement* if_stm = stm();
        nodes.pop();
        Statement* else_stm { nullptr };
        if (peek().type == TokenType::ELSE) {
            consume();
            else_stm = stm();
            nodes.pop();
        }
        s = new Statement(e, if_stm, else_stm);
    } else if (peek().type == TokenType::FOR) {
        consume();
        consume_and_check(TokenType::LPAREN);
        Declaration* d { nullptr };
        Expression* e1 { nullptr };
        if (peek().type == TokenType::INT) { // TODO: Will have to change this, to any type or just call decl() and if it fails revert and check again
            d = decl();
            nodes.pop();
        } else {
            e1 = expr_optional();
            if (e1) {
                nodes.pop();
            }
            consume_and_check(TokenType::SEMICOLON);
        }
        Expression* e2 = expr_optional();
        if (e2) {
            nodes.pop();
        }
        consume_and_check(TokenType::SEMICOLON);
        Expression* e3 = expr_optional();
        if (e3) {
            nodes.pop();
        }
        consume_and_check(TokenType::RPAREN);
        Statement* body = stm();
        nodes.pop();

        if (d)
            s = new Statement(d, e2, e3, body);
        else
            s = new Statement(e1, e2, e3, body);
    } else if (peek().type == TokenType::WHILE) {
        consume();
        consume_and_check(TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop();
        consume_and_check(TokenType::RPAREN);
        Statement* body = stm();
        nodes.pop();
        s = new Statement(Type::WHILE, e, body);
    } else if (peek().type == TokenType::DO) {
        consume();
        Statement* body = stm();
        nodes.pop();
        consume_and_check(TokenType::WHILE);
        consume_and_check(TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop();
        consume_and_check(TokenType::RPAREN);
        consume_and_check(TokenType::SEMICOLON);
        s = new Statement(Type::DO, e, body);
    } else {
        Expression* e = expr_optional();
        if (e) {
            nodes.pop();
        }
        s = new Statement(e, false);
        consume_and_check(TokenType::SEMICOLON);
    }
    nodes.push(s);

    return s;
}

Term* Parser::term()
{
    Term* term { nullptr };

    if (fact()) {
        term = new Term(static_cast<Factor*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::STAR || t.type == TokenType::SLASH) {
            Token op = consume();
            Factor* next_fact = fact();
            nodes.pop();
            term = new Term(next_fact, op, term);
            t = peek();
        }
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
