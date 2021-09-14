#include <iostream>
#include <unordered_map>
#include "lexer.h"

using namespace std;


unordered_map<string, TokenType> keywords {
    { "return",     TokenType::RETURN   },
    { "int",        TokenType::INT      },
    { "if",         TokenType::IF       },
    { "else",       TokenType::ELSE     },
    { "for",        TokenType::FOR      },
    { "while",      TokenType::WHILE    },
    { "do",         TokenType::DO       },
    { "break",      TokenType::BREAK    },
    { "continue",   TokenType::CONTINUE },
};


Lexer::Lexer(std::ifstream& file) : file (file)
{
    ;
}

//---------------------------------------------------------
//   utility functions
//---------------------------------------------------------

char Lexer::consume()
{
    file.read(&c, 1);
    column++;
    return c;
}

//---------------------------------------------------------
//   lexing
//---------------------------------------------------------

std::vector<Token> Lexer::lex()
{
    vector<Token> tokens;

    if (!file.is_open()) {
        cerr << "Must open file to use lexer!" << endl;
        return tokens;
    }

    string current = "";
    while (true) {
        c = file.peek();

        /* check if we have reached the end of the file */
        if (file.eof()) {
            tokens.push_back(Token(TokenType::END_OF_FILE, "EOF"));
            break;
        }

        switch (c) {

        /* symbols */
        case '{':
            consume();
            tokens.push_back(Token(TokenType::LBRACE, "{"));
            break;
        case '}':
            consume();
            tokens.push_back(Token(TokenType::RBRACE, "}"));
            break;
        case '(':
            consume();
            tokens.push_back(Token(TokenType::LPAREN, "("));
            break;
        case ')':
            consume();
            tokens.push_back(Token(TokenType::RPAREN, ")"));
            break;
        case ';':
            consume();
            tokens.push_back(Token(TokenType::SEMICOLON, ";"));
            break;
        case '-':
            consume();
            if (file.peek() == '-') {
                consume();
                tokens.push_back(Token(TokenType::DECREMENT, "--"));
            } else if (file.peek() == '=') {
                consume();
                tokens.push_back(Token(TokenType::MINUS_ASSIGN, "-="));
            } else {
                tokens.push_back(Token(TokenType::MINUS, "-"));
            }
            break;
        case '+':
            consume();
            if (file.peek() == '+') {
                consume();
                tokens.push_back(Token(TokenType::INCREMENT, "++"));
            } else if (file.peek() == '=') {
                consume();
                tokens.push_back(Token(TokenType::PLUS_ASSIGN, "+="));
            } else {
                tokens.push_back(Token(TokenType::PLUS, "+"));
            }
            break;
        case '*':
            consume();
            if (file.peek() == '=') {
                consume();
                tokens.push_back(Token(TokenType::MULT_ASSIGN, "*="));
            } else {
                tokens.push_back(Token(TokenType::STAR, "*"));
            }
            break;
        case '%':
            consume();
            if (file.peek() == '=') {
                consume();
                tokens.push_back(Token(TokenType::MOD_ASSIGN, "%="));
            } else {
                tokens.push_back(Token(TokenType::MODULO, "%"));
            }
            break;
        case '/':
            consume();
            if (file.peek() == '=') {
                consume();
                tokens.push_back(Token(TokenType::DIV_ASSIGN, "*="));
            } else {
                tokens.push_back(Token(TokenType::SLASH, "/"));
            }
            break;
        case '~':
            consume();
            tokens.push_back(Token(TokenType::COMPLEMENT, "~"));
            break;
        case '?':
            consume();
            tokens.push_back(Token(TokenType::QUESTION_MARK, "?"));
            break;
        case ':':
            consume();
            tokens.push_back(Token(TokenType::COLON, ":"));
            break;
        case '!':
            consume();
            c = file.peek();
            if (c == '=') {
                consume();
                tokens.push_back(Token(TokenType::NEQ, "!="));
            } else {
                tokens.push_back(Token(TokenType::NEGATION, "!"));
            }
            break;
        case '&':
            consume();
            c = file.peek();
            if (c == '&') {
                consume();
                tokens.push_back(Token(TokenType::AND, "&&"));
            }
            break;
        case '|':
            consume();
            c = file.peek();
            if (c == '|') {
                consume();
                tokens.push_back(Token(TokenType::OR, "||"));
            }
            break;
        case '=':
            consume();
            c = file.peek();
            if (c == '=') {
                consume();
                tokens.push_back(Token(TokenType::EQ, "=="));
            } else {
                tokens.push_back(Token(TokenType::ASSIGN, "="));
            }
            break;
        case '<':
            consume();
            c = file.peek();
            if (c == '=') {
                consume();
                tokens.push_back(Token(TokenType::LE, "<="));
            } else {
                tokens.push_back(Token(TokenType::LT, "<"));
            }
            break;
        case '>':
            consume();
            c = file.peek();
            if (c == '=') {
                consume();
                tokens.push_back(Token(TokenType::GE, ">="));
            } else {
                tokens.push_back(Token(TokenType::GT, ">"));
            }
            break;

        /* whitespace */
        case ' ':
            column++;
            consume();
            break;
        case '\t':
            column += 4;
            consume();
            break;
        case '\n':
            column = 0;
            line++;
            consume();
            break;

        /* integer literals, identifiers */
        default:
            if (isdigit(c)) {
                current = integer_literal();
                tokens.push_back(Token(TokenType::INTEGER_LITERAL, current));
                current = "";
            } else {
                current = identifier();
                if (!current.empty()) {
                    if (keywords.find(current) != keywords.end()) {
                        tokens.push_back(Token(keywords.at(current), current));
                    } else {
                        tokens.push_back(Token(TokenType::IDENTIFIER, current));
                    }
                }
                else {
                    cerr << "Unexpected character: " << c << endl << "Line: " << line << endl << "Column: " << column << endl;
                    exit(2);
                }
                current = "";
            }
        }
    }

    return tokens;
}

std::string Lexer::identifier()
{
    current_string = "";
    while (true) {
        c = file.peek();
        if (isdigit(c) || isalnum(c) || c == '_') {
            current_string += c;
            consume();
        } else {
            break;
        }
    }
    return current_string;
}

std::string Lexer::integer_literal()
{
    current_string = "";
    while (true) {
        c = file.peek();
        if (isdigit(c)) {
            current_string += c;
            consume();
        } else {
            break;
        }
    }
    return current_string;
}
