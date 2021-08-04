#include "token.h"

std::unordered_map<TokenType, std::string> tokenTypeStrings {
    { TokenType::LBRACE,            "LBRACE",           },
    { TokenType::RBRACE,            "RBRACE",           },
    { TokenType::LPAREN,            "LPAREN",           },
    { TokenType::RPAREN,            "RPAREN",           },
    { TokenType::SEMICOLON,         "SEMICOLON",        },
    { TokenType::RETURN,            "RETURN",           },
    { TokenType::INT,               "INT",              },
    { TokenType::IDENTIFIER,        "IDENTIFIER",       },
    { TokenType::INTEGER_LITERAL,   "INTEGER_LITERAL",  },
    { TokenType::END_OF_FILE,       "END_OF_FILE",      },
    { TokenType::INVALID,           "INVALID",          },
};
