#include <iostream>
#include <fstream>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "lexer.h"
#include "parser.h"
#include "generatorvisitor.h"
#include "node.h"

using namespace std;


void generate(Goal* g);
void print_ast(Goal* g);
vector<string> splitString(string s, char delimiter);

int main(int argc, char** argv)
{
    ifstream input;

    if (argc != 2) {
        return 1;
    }

    string path = argv[1];
    string filename = splitString(path, '/').back();
    filename.resize(filename.length() - 2);
    string new_file_path = path;
    new_file_path.resize(path.length() - 2);

    input.open(path);

    Lexer lex(input);
    auto tokens = lex.lex();
    Parser parser(tokens);
    auto g = parser.parse();
//    print_ast(g);
    generate(g);

    pid_t pid = fork();
    if (pid == 0) {
        int i = execl("/usr/bin/gcc", "gcc", "-m32", "./assembly.s", "-o", filename.c_str(), NULL);
        if (i) {
            std::cout << errno << std::endl;
        }
    } else if (pid > 0) {
        int status;
        wait(&status);
    }

    pid = fork();
    if (pid == 0) {
        int i = execl("/usr/bin/cp", "cp", string("./" + filename).c_str(), new_file_path.c_str(), NULL);
        if (i) {
            std::cout << errno << std::endl;
        }
    } else if (pid > 0) {
        int status;
        wait(&status);
    }

//    remove("assembly.s");

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
    g->print_node(0);
}

vector<string> splitString(string s, char delimiter)
{
    vector<string> strings {};
    string currentString = "";
    for (auto x : s) {
        if (x == delimiter) {
            strings.push_back(currentString);
            currentString = "";
        } else {
            currentString += x;
        }
    }
    if (currentString != "") {
        strings.push_back(currentString);
    }

    return strings;
}


