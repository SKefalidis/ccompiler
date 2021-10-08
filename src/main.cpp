#include <iostream>
#include <fstream>

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "lexer.h"
#include "parser.h"
#include "generatorvisitor.h"
#include "functiondeclarationvisitor.h"
#include "blockgenerator.h"
#include "peepholeoptimizer.h"
#include "controlflowgraph.h"
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
    FunctionDeclarationVisitor fvisitor;
    fvisitor.visit(g);
    generate(g);

    /* optimize */
    input.close();
    input.open("./assembly.s");
    BlockGenerator blocker(input);
    auto blocks = blocker.get_blocks();

//    for (auto& b : blocks) {
//        if (b->is_code_block()) {
//            std::cout << "Basic Block:" << std::endl;
//            for (auto& i : b->instructions) {
//                std::cout << i->to_string() << std::endl;
//            }
//        }
//    }

    ControlFlowGraph grapher(blocks);
    blocks = grapher.remove_dead_blocks();

//    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

//    for (auto& b : grapher.remove_dead_blocks()) {
//        if (b->is_code_block()) {
//            std::cout << "Basic Block:" << std::endl;
//            for (auto& i : b->instructions) {
//                std::cout << i->to_string() << std::endl;
//            }
//        }
//    }

    PeepholeOptimizer peeophole_optimizer;
    peeophole_optimizer.optimize(blocks);
    input.close();

//    std::cout << "Optimized" << std::endl;
//    for (auto& b : blocks) {
//        if (!b->is_code_block()) {
//            continue;
//        }
//        std::cout << "Basic Block:" << std::endl;
//        for (auto& i : b->instructions) {
//            std::cout << i->to_string() << std::endl;
//        }
//    }

    /* write optimized assembly file */
    ofstream output;
    output.open("optimized_assembly.s");
    for (auto& b : blocks) {
        for (auto& l : b->lines) {
            output << l << std::endl;
        }
        for (auto& i : b->instructions) {
            output << i->to_string() << std::endl;
        }
    }
    output.close();

    /* create executable from x86 assembly */
    pid_t pid = fork();
    if (pid == 0) {
        int i = execl("/usr/bin/gcc", "gcc", "-m32", "./optimized_assembly.s", "-o", filename.c_str(), NULL);
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
