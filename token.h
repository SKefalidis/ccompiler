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
    PLUS,
    STAR,
    SLASH,
    AND,
    OR,
    EQ,
    NEQ,
    LT,
    LE,
    GT,
    GE,
    ASSIGN,

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
    Token()
    {
        type = TokenType::INVALID;
        value = tokenTypeStrings.at(type);
    }
    Token(TokenType type, std::string value)
    {
        this->type = type;
        this->value = value;
    }

    TokenType   type;
    std::string value;
};

#endif // TOKEN_H
