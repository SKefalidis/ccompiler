#include <iostream>
#include <unordered_map>
#include "lexer.h"

using namespace std;



unordered_map<string, TokenType> keywords {
    { "return",  TokenType::RETURN  },
    { "int",     TokenType::INT     },
};

Lexer::Lexer(std::ifstream& file) : file (file)
{
    ;
}

std::vector<Token> Lexer::lex()
{
    vector<Token> tokens;

    if (!file.is_open()) {
        cerr << "Must open file to user lexer!" << endl;
        return tokens;
    }

    string current = "";
    while (true) {
        c = file.peek();

        if (file.eof())
            break;

        switch (c) {

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

        default:
            if (isdigit(c)) {
                current = integer_literal();
                tokens.push_back(Token(TokenType::INT, current));
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

char Lexer::consume()
{
    file.read(&c, 1);
    column++;
    return c;
}
