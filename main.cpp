#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

using namespace std;



int main(int argc, char** argv)
{
    ifstream input;

    if (argc != 2) {
        return 1;
    }

    input.open(argv[1]);

    Lexer lex(input);
    auto tokens = lex.lex();
    Parser parser(tokens);
    auto g = parser.parse();

    for (auto& x : tokens) {
        cout << x.value << endl;
    }

    return 0;
}
