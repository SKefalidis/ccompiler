#include <iostream>
#include <unordered_set>
#include "parser.h"


Parser::Parser(std::vector<Token> tokens) : tokens(tokens)
{
    ;
}

//---------------------------------------------------------
//   utility functions
//---------------------------------------------------------

#define NULL_CHECK(x)       \
    if (x == NULL) {        \
        backtrack();        \
        return nullptr;     \
    }

Token Parser::peek(int offset) const
{
    return tokens.at(current_token + offset);
}

/* Like peek but also logs errors */
bool Parser::expect(TokenType expected, int offset)
{
    bool result = expected == peek(offset).type;
    if (result == false)
        errors.push_back(std::pair<std::string, int>(tokenTypeStrings.at(expected), current_token + offset));
    return result;
}

bool Parser::expect(std::vector<TokenType> expected, int offset)
{
    bool result = false;
    for (auto& x : expected) {
        if (x == peek(offset).type) {
            return true;
        }
    }

    std::string error {""};
    for (auto& x : expected) {
        error += tokenTypeStrings.at(x) + " ";
    }
    errors.push_back(std::pair<std::string, int>(error, current_token + offset));

    return false;
}

Token Parser::consume()
{
    tokens_used.top()++;
    return tokens.at(current_token++);
}

/* consume with added error handling */
#define CONSUME_AND_CHECK(t, expected)                                                                  \
    t = consume();                                                                                      \
    if (t.type != expected) {                                                                           \
        errors.push_back(std::pair<std::string, int>(tokenTypeStrings.at(expected), -(current_token - 1)));\
        backtrack();                                                                                    \
        return nullptr;                                                                                 \
    }

void Parser::backtrack()
{
    current_token -= tokens_used.top();
    tokens_used.pop();
}

void Parser::backtrack(int i)
{
    current_token -= i;
    tokens_used.top() -= i;
    if (tokens_used.top() < 0) {
        throw "Incorrect backtracking";
    }
}

/* There are 2 kinds of error logged, peeking and consumption errors.
 * Consumption errors have a negative value and are prioritized.
 */
void Parser::parse_error()
{
    std::string error = errors.at(0).first;

    int tokens_consumed = abs(errors.at(0).second);
    for (auto& e : errors) {
        if (abs(e.second) > tokens_consumed) {
            error = e.first;
            tokens_consumed = abs(e.second);
        }
    }

    std::unordered_set<std::string> pos_expected {};
    for (auto& e : errors) {
        if (e.second == tokens_consumed) {
            pos_expected.insert(e.first);
        }
    }

    std::unordered_set<std::string> neg_expected {};
    for (auto& e : errors) {
        if (e.second == -tokens_consumed) {
            neg_expected.insert(e.first);
        }
    }

    std::cerr << "Expected one of the following tokens: " << std::endl;
    if (neg_expected.size() > 0) {
        for (auto& e : neg_expected) {
            std::cerr << e << std::endl;
        }
    } else {
        for (auto& e : pos_expected) {
            std::cerr << e << std::endl;
        }
    }

    throw "Unexpected character";
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
            decls.push_back(f);
        } else if (Declaration *d = decl()) {
            decls.push_back(d);
        } else {
            parse_error();
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
    Term* next_term       { nullptr };
    tokens_used.push(0);

    if ((next_term = term())) {
        e = new AdditiveExpression(next_term);
        while (expect({TokenType::PLUS, TokenType::MINUS})) {
            Token op = consume();
            next_term = term();
            NULL_CHECK(next_term)
            e = new AdditiveExpression(next_term, op, e);
        }
        tokens_used.pop();
        return e;
    } else {
        backtrack();
        return nullptr;
    }
}

AndExpression* Parser::and_expr()
{
    AndExpression* e { nullptr };
    EqualityExpression* next_expr { nullptr };
    tokens_used.push(0);

    if ((next_expr = eq_expr())) {
        e = new AndExpression(next_expr);
        while (expect(TokenType::AND)) {
            Token op = consume();
            next_expr = eq_expr();
            NULL_CHECK(next_expr)
            e = new AndExpression(next_expr, op, e);
        }
        tokens_used.pop();
        return e;
    } else {
        backtrack();
        return nullptr;
    }
}

BlockItem* Parser::block_item()
{
    BlockItem* b { nullptr };
    tokens_used.push(0);

    if (Declaration* d = decl()) {
        b = new BlockItem(d);
    } else if (Statement* s = stm()) {
        b = new BlockItem(s);
    } else {
        backtrack();
        return nullptr;
    }

    tokens_used.pop();
    return b;
}

CondExpression* Parser::cond_expr()
{
    CondExpression* e { nullptr };
    Token t;
    tokens_used.push(0);

    if (OrExpression* or_e = or_expr()) {
        Expression* if_expr { nullptr };
        CondExpression* else_expr { nullptr };
        if (expect(TokenType::QUESTION_MARK)) {
            consume();
            if_expr = expr();
            NULL_CHECK(if_expr)
            CONSUME_AND_CHECK(t, TokenType::COLON);
            else_expr = cond_expr();
            NULL_CHECK(else_expr)
        }
        e = new CondExpression(or_e, if_expr, else_expr);
        tokens_used.pop();
        return e;
    } else {
        backtrack();
        return nullptr;
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
    if (expect(TokenType::ASSIGN)) {
        consume();
        e = expr();
        NULL_CHECK(e)
    }
    CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
    d = new Declaration(id, e);

    tokens_used.pop();
    return d;
}

EqualityExpression* Parser::eq_expr()
{
    EqualityExpression* e { nullptr };
    RelationalExpression* next_expr { nullptr };
    tokens_used.push(0);

    if ((next_expr = rel_expr())) {
        e = new EqualityExpression(next_expr);
        while (expect({TokenType::EQ, TokenType::NEQ})) {
            Token op = consume();
            next_expr = rel_expr();
            NULL_CHECK(next_expr)
            e = new EqualityExpression(next_expr, op, e);
        }
        tokens_used.pop();
        return e;
    } else {
        backtrack();
        return nullptr;
    }
}

Expression* Parser::expr()
{
    Expression* e { nullptr };
    tokens_used.push(0);

    if (expect(TokenType::IDENTIFIER)) {
        std::string id = consume().value;
        if (expect({TokenType::ASSIGN, TokenType::PLUS_ASSIGN, TokenType::MINUS_ASSIGN,
                   TokenType::MULT_ASSIGN, TokenType::DIV_ASSIGN, TokenType::MOD_ASSIGN})) {
            Token t = consume(); /* consume (x)-assign operator */

            /* get the operator if the operation is not a simple assignment */
            switch (t.type) {
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

            Expression* r_expr = expr();
            NULL_CHECK(r_expr)
            e = new Expression(id, r_expr, t);
            tokens_used.pop();
            return e;
        }
    }

    backtrack();
    tokens_used.push(0);

    if (CondExpression* expr = cond_expr()) {
        e = new Expression(expr);
        tokens_used.pop();
        return e;
    }

    return nullptr;
}

Expression* Parser::expr_optional()
{
    tokens_used.push(0);

    if (expect({TokenType::SEMICOLON, TokenType::RPAREN})) {
        tokens_used.pop();
        return new Expression(nullptr); /* return empty expression */
    } else if (Expression* e = expr()) {
        tokens_used.pop();
        return e;
    } else {
        backtrack();
        return nullptr;
    }
}

OrExpression* Parser::or_expr()
{
    OrExpression* e { nullptr };
    AndExpression* next_expr { nullptr };
    tokens_used.push(0);

    if ((next_expr = and_expr())) {
        e = new OrExpression(next_expr);
        while (expect(TokenType::OR)) {
            Token op = consume();
            next_expr = and_expr();
            NULL_CHECK(next_expr)
            e = new OrExpression(next_expr, op, e);
        }
    } else {
        backtrack();
        return nullptr;
    }

    tokens_used.pop();
    return e;
}

Factor* Parser::fact()
{
    Factor* f { nullptr };
    Factor* inner_factor { nullptr };
    UnaryOperator* op { nullptr };
    Token t;
    tokens_used.push(0);

    if (expect(TokenType::INTEGER_LITERAL)) {
        t = consume();
        f = new Factor(new IntLiteral(t.value));
    } else if (expect(TokenType::IDENTIFIER)) {
        if (expect(TokenType::LPAREN, 1)) {
            FunctionCall* func = func_call();
            NULL_CHECK(func)
            f = new Factor(func);
        } else {
            CONSUME_AND_CHECK(t, TokenType::IDENTIFIER)
            f = new Factor(t.value);
        }
    } else if (expect(TokenType::LPAREN)) {
        consume();
        Expression* e = expr();
        NULL_CHECK(e)
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        f = new Factor(e);
    } else if ((op = unary_op()) && (inner_factor = fact())) {
        f = new Factor(op, inner_factor);
    } else {
        backtrack();
        return nullptr;
    }

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
    if (!expect(TokenType::RPAREN)) {
        while (true) {
            if (!expect(TokenType::INT)) {
                break;
            } else {
                consume();
            }
            if (expect(TokenType::IDENTIFIER)) {
                parameters.push_back(std::pair<std::string, std::string>("INT", consume().value));
            }
            if (expect(TokenType::RPAREN)) {
                break;
            } else {
                CONSUME_AND_CHECK(t, TokenType::COMMA);
            }
        }
    }
    CONSUME_AND_CHECK(t, TokenType::RPAREN);
    if (expect(TokenType::SEMICOLON)) {
        consume();
        f = new FunctionDeclaration(name, parameters);
    } else {
        /* also a definition */
        CONSUME_AND_CHECK(t, TokenType::LBRACE);
        std::vector<BlockItem*> items {};
        while (!expect(TokenType::RBRACE)) {
            if (BlockItem* b = block_item()) {
                items.push_back(b);
            } else {
                backtrack();
                return nullptr;
            }
        }
        CONSUME_AND_CHECK(t, TokenType::RBRACE);
        f = new FunctionDeclaration(name, parameters, items);
    }

    tokens_used.pop();
    return f;
}

FunctionCall* Parser::func_call()
{
    FunctionCall* f { nullptr };
    Token t;
    tokens_used.push(0);

    CONSUME_AND_CHECK(t, TokenType::IDENTIFIER);
    std::string id = t.value;
    CONSUME_AND_CHECK(t, TokenType::LPAREN);
    std::vector<Expression*> expressions {};
    if (!expect(TokenType::RPAREN)) {
        while (true) {
            if (Expression* e = expr()) {
                expressions.push_back(e);
            } else {
                backtrack();
                return nullptr;
            }
            if (expect(TokenType::RPAREN)) {
                break;
            } else {
                CONSUME_AND_CHECK(t, TokenType::COMMA);
            }
        }
    }
    CONSUME_AND_CHECK(t, TokenType::RPAREN);
    f = new FunctionCall(id, expressions);

    tokens_used.pop();
    return f;
}

RelationalExpression* Parser::rel_expr()
{
    RelationalExpression* e { nullptr };
    AdditiveExpression* next_expr { nullptr };
    tokens_used.push(0);

    if ((next_expr = add_expr())) {
        e = new RelationalExpression(next_expr);
        while (expect({TokenType::LT, TokenType::LE, TokenType::GT, TokenType::GE})) {
            Token op = consume();
            next_expr = add_expr();
            NULL_CHECK(next_expr)
            e = new RelationalExpression(next_expr, op, e);
        }
    } else {
        backtrack();
        return nullptr;
    }

    tokens_used.pop();
    return e;
}

Statement* Parser::stm()
{
    Statement* s { nullptr };
    Token t;
    tokens_used.push(0);

    if (expect(TokenType::RETURN)) {
        consume();
        if (Expression* e = expr()) {
            CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
            s = new Statement(e, true);
        } else {
            backtrack();
            return nullptr;
        }
    } else if (expect(TokenType::BREAK)) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(Type::BREAK);
    } else if (expect(TokenType::CONTINUE)) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(Type::CONTINUE);
    } else if (expect(TokenType::LBRACE)) {
        consume();
        std::vector<BlockItem*> block_items;
        while (!expect(TokenType::RBRACE)) {
            if (BlockItem* b = block_item()) {
                block_items.push_back(b);
            } else {
                return nullptr;
            }
        }
        CONSUME_AND_CHECK(t, TokenType::RBRACE);
        s = new Statement(block_items);
    } else if (expect(TokenType::IF)) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Expression* e = expr();
        NULL_CHECK(e)
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        Statement* if_stm = stm();
        NULL_CHECK(if_stm)
        Statement* else_stm { nullptr };
        if (expect(TokenType::ELSE)) {
            consume();
            else_stm = stm();
            NULL_CHECK(else_stm)
        }
        s = new Statement(e, if_stm, else_stm);
    } else if (expect(TokenType::FOR)) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Declaration* d { nullptr };
        Expression* e1 { nullptr };
        if ((d = decl())) {
            ;
        } else {
            e1 = expr_optional();
            NULL_CHECK(e1)
            CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        }
        Expression* e2 = expr_optional();
        NULL_CHECK(e2)
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        Expression* e3 = expr_optional();
        NULL_CHECK(e3)
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        Statement* body = stm();
        NULL_CHECK(body)

        if (d)
            s = new Statement(d, e2, e3, body);
        else
            s = new Statement(e1, e2, e3, body);
    } else if (expect(TokenType::WHILE)) {
        consume();
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Expression* e = expr();
        NULL_CHECK(e)
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        Statement* body = stm();
        NULL_CHECK(body)
        s = new Statement(Type::WHILE, e, body);
    } else if (expect(TokenType::DO)) {
        consume();
        Statement* body = stm();
        NULL_CHECK(body)
        CONSUME_AND_CHECK(t, TokenType::WHILE);
        CONSUME_AND_CHECK(t, TokenType::LPAREN);
        Expression* e = expr();
        NULL_CHECK(e)
        CONSUME_AND_CHECK(t, TokenType::RPAREN);
        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(Type::DO, e, body);
    } else {
        Expression* e = expr_optional();
        NULL_CHECK(e)

        CONSUME_AND_CHECK(t, TokenType::SEMICOLON);
        s = new Statement(e, false);
    }

    tokens_used.pop();
    return s;
}

Term* Parser::term()
{
    Term* term { nullptr };
    Factor* factor { nullptr };
    tokens_used.push(0);

    if ((factor = fact())) {
        term = new Term(factor);
        while (expect(TokenType::STAR) || expect(TokenType::SLASH) || expect(TokenType::MODULO)) {
            Token op = consume();
            factor = fact();
            NULL_CHECK(factor)
            term = new Term(factor, op, term);
        }
        tokens_used.pop();
        return term;
    } else {
        backtrack();
        return nullptr;
    }
}

UnaryOperator* Parser::unary_op()
{
    UnaryOperator* op { nullptr };
    tokens_used.push(0);

    if (expect({TokenType::COMPLEMENT, TokenType::MINUS, TokenType::NEGATION})) {
        Token t = consume();
        op = new UnaryOperator(t);
    } else if (expect({TokenType::INCREMENT, TokenType::DECREMENT})) {
        Token t = consume();
        std::string id { "" };
        for (int i = 0; true; i++) {
            if (expect(TokenType::IDENTIFIER, i)) {
                id = tokens.at(current_token + i).value;
                break;
            }
        }
        op = new UnaryOperator(t, id);
    } else {
        backtrack();
        return nullptr;
    }

    tokens_used.pop();
    return op;
}
