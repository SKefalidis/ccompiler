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
    tokens_used.top()++;
    return tokens.at(current_token++);
}

#define BACKTRACK   \
    backtrack();    \
    return nullptr;

#define CONSUME_AND_CHECK(t, expected)                  \
    t = consume();                                      \
    if (t.type != expected) {                           \
        error = tokenTypeStrings.at(expected);          \
        BACKTRACK;                                      \
    }

void Parser::backtrack()
{
    current_token -= tokens_used.top();
    tokens_used.pop();
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
    Token t;
    tokens_used.push(0);

    current_token = 0;
    std::vector<Node*> decls {};
    while (peek().type != TokenType::END_OF_FILE) {
        if (FunctionDeclaration* f = func_decl()) {
            nodes.pop();
            decls.push_back(f);
        } else if (Declaration *d = decl()) {
            nodes.pop();
            decls.push_back(d);
        } else {
            parse_error(error);
        }
    }
    CONSUME_AND_CHECK(t, TokenType::END_OF_FILE);
    g = new Goal(decls);

    tokens_used.pop();
    return g;
}

AdditiveExpression* Parser::add_expr()
{
    AdditiveExpression* e { nullptr };
    tokens_used.push(0);

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
        tokens_used.pop();
        return e;
    } else {
        BACKTRACK
    }
}

AndExpression* Parser::and_expr()
{
    AndExpression* e { nullptr };
    tokens_used.push(0);

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
        tokens_used.pop();
        return e;
    } else {
        BACKTRACK
    }
}

BlockItem* Parser::block_item()
{
    BlockItem* b { nullptr };
    tokens_used.push(0);

    if (decl()) {
        b = new BlockItem(static_cast<Declaration*>(get_and_pop()));
    } else if (stm()) {
        b = new BlockItem(static_cast<Statement*>(get_and_pop()));
    } else {
        BACKTRACK
    }
    nodes.push(b);

    tokens_used.pop();
    return b;
}

CondExpression* Parser::cond_expr()
{
    CondExpression* e { nullptr };
    Token t;
    tokens_used.push(0);

    if (or_expr()) {
        OrExpression* or_expr = static_cast<OrExpression*>(get_and_pop());
        Expression* if_expr { nullptr };
        CondExpression* else_expr { nullptr };
        if (peek().type == TokenType::QUESTION_MARK) {
            consume();
            if_expr = expr();
            CONSUME_AND_CHECK(t, TokenType::COLON);
            else_expr = cond_expr();
        }
        e = new CondExpression(or_expr, if_expr, else_expr);
        nodes.push(e);
        tokens_used.pop();
        return e;
    } else {
        BACKTRACK
    }
}

Declaration* Parser::decl()
{
    Declaration* d { nullptr };
    Token t;
    tokens_used.push(0);

    CONSUME_AND_CHECK(t, TokenType::INT);
    CONSUME_AND_CHECK(t, TokenType::IDENTIFIER);
    std::string id = t.value;
    Expression* e = nullptr;
    if (peek().type == TokenType::ASSIGN) {
        consume();
        e = expr();
        nodes.pop();
    }
    CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
    d = new Declaration(id, e);
    nodes.push(d);

    tokens_used.pop();
    return d;
}

EqualityExpression* Parser::eq_expr()
{
    EqualityExpression* e { nullptr };
    tokens_used.push(0);

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
        tokens_used.pop();
        return e;
    } else {
        BACKTRACK
    }
}

Expression* Parser::expr()
{
    Expression* e { nullptr };
    tokens_used.push(0);

    if (peek().type == TokenType::IDENTIFIER &&
            (peek(1).type == TokenType::ASSIGN
            || peek(1).type == TokenType::PLUS_ASSIGN
            || peek(1).type == TokenType::MINUS_ASSIGN
            || peek(1).type == TokenType::MULT_ASSIGN
            || peek(1).type == TokenType::DIV_ASSIGN
            || peek(1).type == TokenType::MOD_ASSIGN)) {
        Token t;

        /* get the operator if the operation is not a simple assignment */
        switch (peek(1).type) {
        case TokenType::PLUS_ASSIGN:
            t = Token(TokenType::PLUS, tokenTypeStrings.at(TokenType::PLUS));
            break;
        case TokenType::MINUS_ASSIGN:
            t = Token(TokenType::MINUS, tokenTypeStrings.at(TokenType::MINUS));
            break;
        case TokenType::MULT_ASSIGN:
            t = Token(TokenType::STAR, tokenTypeStrings.at(TokenType::STAR));
            break;
        case TokenType::DIV_ASSIGN:
            t = Token(TokenType::SLASH, tokenTypeStrings.at(TokenType::SLASH));
            break;
        case TokenType::MOD_ASSIGN:
            t = Token(TokenType::MODULO, tokenTypeStrings.at(TokenType::MODULO));
            break;
        default:
            t = Token(TokenType::INVALID, tokenTypeStrings.at(TokenType::INVALID));
        }

        std::string id = consume().value;
        consume(); /* consume (x)-assign operator */
        Expression* r_expr = expr();
        nodes.pop();
        e = new Expression(id, r_expr, t);
        nodes.push(e);
    } else if (cond_expr()) {
        CondExpression* expr = static_cast<CondExpression*>(get_and_pop());
        e = new Expression(expr);
        nodes.push(e);
    } else {
        BACKTRACK
    }

    return e;
}

Expression* Parser::expr_optional()
{
    tokens_used.push(0);

    if (peek().type == TokenType::SEMICOLON || peek().type == TokenType::RPAREN) {
        tokens_used.pop();
        return nullptr;
    } else if (Expression* e = expr()) {
        tokens_used.pop();
        return e;
    } else {
        BACKTRACK
    }
}

OrExpression* Parser::or_expr()
{
    OrExpression* e { nullptr };
    tokens_used.push(0);

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
    } else {
        BACKTRACK
    }

    tokens_used.pop();
    return e;
}

Factor* Parser::fact()
{
    Factor* f { nullptr };
    Token t;
    tokens_used.push(0);

    if (peek().type == TokenType::INTEGER_LITERAL) {
        t = consume();
        f = new Factor(new IntLiteral(t.value));
    } else if (peek().type == TokenType::IDENTIFIER) {
        if (peek(1).type == TokenType::LPAREN) {
            FunctionCall* func = func_call();
            nodes.pop();
            f = new Factor(func);
        } else {
            std::string variable = consume().value;
            f = new Factor(variable);
        }
    } else if (peek().type == TokenType::LPAREN) {
        consume();
        Expression* e = expr();
        nodes.pop();
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        f = new Factor(e);
    } else if (unary_op() && fact()) {
        Factor* inner_factor = static_cast<Factor*>(get_and_pop());
        UnaryOperator* op = static_cast<UnaryOperator*>(get_and_pop());
        f = new Factor(op, inner_factor);
    } else {
        BACKTRACK
    }
    nodes.push(f);

    tokens_used.pop();
    return f;
}

FunctionDeclaration* Parser::func_decl()
{
    FunctionDeclaration* f  { nullptr };
    Token t;
    tokens_used.push(0);

    CONSUME_AND_CHECK(t, TokenType::INT);
    CONSUME_AND_CHECK(t, TokenType::IDENTIFIER);
    std::string name = t.value;
    CONSUME_AND_CHECK(t, TokenType::LPAREN);
    std::vector<std::pair<std::string, std::string>> parameters {};
    if (peek().type != TokenType::RPAREN) {
        while (true) {
            CONSUME_AND_CHECK(t, TokenType::INT);
            if (peek().type == TokenType::IDENTIFIER) {
                parameters.push_back(std::pair<std::string, std::string>("INT", peek().value));
                consume();
            }
            if (peek().type == TokenType::RPAREN) {
                break;
            } else {
                CONSUME_AND_CHECK(t, TokenType::COMMA);
            }
        }
    }
    CONSUME_AND_CHECK(t, TokenType::RPAREN);
    if (peek().type == TokenType::SEMICOLON) {
        consume();
        f = new FunctionDeclaration(name, parameters);
    } else {
        /* also a definition */
        CONSUME_AND_CHECK(t, TokenType::LBRACE);
        std::vector<BlockItem*> items {};
        while (peek().type != TokenType::RBRACE) {
            if (block_item()) {
                BlockItem* b = static_cast<BlockItem*>(get_and_pop()); /* TODO: use `auto` */
                items.push_back(b);
            } else {
                BACKTRACK
            }
        }
        CONSUME_AND_CHECK(t, TokenType::RBRACE);
        f = new FunctionDeclaration(name, parameters, items);
    }
    nodes.push(f);

    tokens_used.pop();
    return f;
}

FunctionCall* Parser::func_call()
{
    FunctionCall* f { nullptr };
    Token t;

    CONSUME_AND_CHECK(t, TokenType::IDENTIFIER);
    std::string id = t.value;
    CONSUME_AND_CHECK(t, TokenType::LPAREN);
    std::vector<Expression*> expressions {};
    if (peek().type != TokenType::RPAREN) {
            while (true) {
            if (expr()) {
                Expression* e = static_cast<Expression*>(get_and_pop());
                expressions.push_back(e);
            } else {
                parse_error("Expected argument");
            }
            if (peek().type == TokenType::RPAREN) {
                break;
            } else {
                CONSUME_AND_CHECK(t, TokenType::COMMA);
            }
        }
    }
    CONSUME_AND_CHECK(t, TokenType::RPAREN);
    f = new FunctionCall(id, expressions);
    nodes.push(f);

    return f;
}

RelationalExpression* Parser::rel_expr()
{
    RelationalExpression* e { nullptr };
    tokens_used.push(0);

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
    } else {
        BACKTRACK
    }

    tokens_used.pop();
    return e;
}

Statement* Parser::stm()
{
    Statement* s { nullptr };
    Token t;
    tokens_used.push(0);

    if (peek().type == TokenType::RETURN) {
        consume();
        Expression* e = expr();
        nodes.pop();
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(e, true);
    } else if (peek().type == TokenType::BREAK) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(Type::BREAK);
    } else if (peek().type == TokenType::CONTINUE) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
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
        CONSUME_AND_CHECK(t, TokenType::RBRACE);
        s = new Statement(block_items);
    } else if (peek().type == TokenType::IF) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop();
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
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
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
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
            CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        }
        Expression* e2 = expr_optional();
        if (e2) {
            nodes.pop();
        }
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        Expression* e3 = expr_optional();
        if (e3) {
            nodes.pop();
        }
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        Statement* body = stm();
        nodes.pop();

        if (d)
            s = new Statement(d, e2, e3, body);
        else
            s = new Statement(e1, e2, e3, body);
    } else if (peek().type == TokenType::WHILE) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop();
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        Statement* body = stm();
        nodes.pop();
        s = new Statement(Type::WHILE, e, body);
    } else if (peek().type == TokenType::DO) {
        consume();
        Statement* body = stm();
        nodes.pop();
        CONSUME_AND_CHECK(t, TokenType::WHILE);
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Expression* e = expr();
        nodes.pop();
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(Type::DO, e, body);
    } else {
        Expression* e = expr_optional();
        if (e) {
            nodes.pop();
        }
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(e, false);
    }
    nodes.push(s);

    tokens_used.pop();
    return s;
}

Term* Parser::term()
{
    Term* term { nullptr };
    tokens_used.push(0);

    if (fact()) {
        term = new Term(static_cast<Factor*>(get_and_pop()));
        Token t = peek();
        while (t.type == TokenType::STAR || t.type == TokenType::SLASH || t.type == TokenType::MODULO) {
            Token op = consume();
            Factor* next_fact = fact();
            nodes.pop();
            term = new Term(next_fact, op, term);
            t = peek();
        }
        nodes.push(term);
        tokens_used.pop();
        return term;
    } else {
        BACKTRACK
    }
}

UnaryOperator* Parser::unary_op()
{
    UnaryOperator* op { nullptr };
    tokens_used.push(0);

    auto type = peek().type;
    if (type == TokenType::COMPLEMENT || type == TokenType::MINUS || type == TokenType::NEGATION) {
        Token t = consume();
        op = new UnaryOperator(t);
        nodes.push(op);
    } else if (type == TokenType::INCREMENT || type == TokenType::DECREMENT) {
        Token t = consume();
        std::string id { "" };
        for (int i = 0; true; i++) {
            if (peek(i).type == TokenType::IDENTIFIER) {
                id = peek(i).value;
                break;
            }
        }
        op = new UnaryOperator(t, id);
        nodes.push(op);
    } else {
        BACKTRACK
    }

    tokens_used.pop();
    return op;
}
