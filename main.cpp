#include <iostream>
#include <fstream>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "lexer.h"
#include "parser.h"
#include "generatorvisitor.h"

using namespace std;


void generate(Goal* g);
void print_ast(Goal* g);

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

    pid_t pid = fork();
    if (pid == 0) {
        int i = execl("/usr/bin/gcc", "gcc", "-m32", "./assembly.s", "-o", "out", NULL);
        if (i) {
            std::cout << errno << std::endl;
        }
    } else if (pid > 0) {
        int status;
        wait(&status);
    }

    remove("assembly.s");

    return 0;
}

void generate(Goal* g)
{
    ofstream output;
    output.open("assembly.s");
    GeneratorVisitor v(output);
    v.visit(g);
    output.close();
}

void print_ast(Goal* g)
{

}
