#include <iostream>
#include "function.h"

Function::Function(std::string name)
    : name(name)
{
    ;
}

Function::Function(std::string name, std::vector<Statement*> stm)
    : name(name), stm(stm)
{
    ;
}

void Function::accept(Visitor* v)
{
    v->visit(this);
}

void Function::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "FUNCTION" << std::endl;
    std::cout << tabs_string(tabs) << name << std::endl;
    for (auto& x : stm) {
        x->print_node(tabs + 1);
    }
}
