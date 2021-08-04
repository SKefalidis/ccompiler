#include <iostream>
#include <fstream>
#include "lexer.h"

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

    for (auto& x : tokens) {
        cout << x.value << endl;
    }

    return 0;
}
