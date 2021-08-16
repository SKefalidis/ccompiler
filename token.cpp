#include "token.h"

std::unordered_map<TokenType, std::string> tokenTypeStrings {
    { TokenType::LBRACE,            "LBRACE",           },
    { TokenType::RBRACE,            "RBRACE",           },
    { TokenType::LPAREN,            "LPAREN",           },
    { TokenType::RPAREN,            "RPAREN",           },
    { TokenType::SEMICOLON,         "SEMICOLON",        },
    { TokenType::MINUS,             "MINUS",            },
    { TokenType::COMPLEMENT,        "COMPLEMENT",       },
    { TokenType::NEGATION,          "NEGATION",         },
    { TokenType::PLUS,              "PLUS",             },
    { TokenType::STAR,              "STAR",             },
    { TokenType::SLASH,             "SLASH",            },
    { TokenType::AND,               "AND",              },
    { TokenType::OR,                "OR",               },
    { TokenType::EQ,                "EQ",               },
    { TokenType::NEQ,               "NEQ",              },
    { TokenType::LT,                "LT",               },
    { TokenType::LE,                "LE",               },
    { TokenType::GT,                "GT",               },
    { TokenType::GE,                "GE",               },
    { TokenType::ASSIGN,            "ASSIGN",           },

    { TokenType::RETURN,            "RETURN",           },
    { TokenType::INT,               "INT",              },

    { TokenType::IDENTIFIER,        "IDENTIFIER",       },
    { TokenType::INTEGER_LITERAL,   "INTEGER_LITERAL",  },

    { TokenType::END_OF_FILE,       "END_OF_FILE",      },
    { TokenType::INVALID,           "INVALID",          },
};
