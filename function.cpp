#include <iostream>
#include "function.h"

Function::Function(std::string name, Statement* stm)
    : name(name), stm(stm)
{

}

void Function::accept(Visitor* v)
{
    v->visit(this);
}

void Function::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "FUNCTION" << std::endl;
    std::cout << tabs_string(tabs) << name << std::endl;
    stm->print_node(tabs + 1);
}
