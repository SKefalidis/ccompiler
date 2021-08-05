#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "generatorvisitor.h"

using namespace std;


void generate(Goal* g);

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
    generate(g);

//    for (auto& x : tokens) {
//        cout << x.value << endl;
//    }

    return 0;
}

void generate(Goal* g)
{
    ofstream output;
    output.open("assembly.s");
    GeneratorVisitor v(output);
    v.visit(g);
}
