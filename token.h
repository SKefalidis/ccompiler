#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    SEMICOLON,
    RETURN,
    INT,
    IDENTIFIER,
    INTEGER_LITERAL,
};

struct Token
{
    Token(TokenType type, std::string value)
    {
        this->type = type;
        this->value = value;
    }

    TokenType   type;
    std::string value;
};

#endif // TOKEN_H
