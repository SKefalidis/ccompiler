#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <fstream>

#include "token.h"


class Lexer
{
public:
    Lexer(std::ifstream& file);

    std::vector<Token> lex ();

private:
    std::string identifier      ();
    std::string integer_literal ();
    char        consume         ();

    std::ifstream&  file;
    std::string     current_string;
    char            c;

    int line    { 0 };
    int column  { 0 };
};

#endif // LEXER_H
