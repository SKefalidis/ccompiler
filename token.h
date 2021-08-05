#ifndef TOKEN_H
#define TOKEN_H

#include <unordered_map>
#include <string>


enum class TokenType {
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    SEMICOLON,
    MINUS,
    COMPLEMENT,
    NEGATION,

    RETURN,
    INT,

    IDENTIFIER,
    INTEGER_LITERAL,

    END_OF_FILE,
    INVALID
};

extern std::unordered_map<TokenType, std::string> tokenTypeStrings;

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
