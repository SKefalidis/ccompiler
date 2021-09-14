#include <iostream>
#include "function.h"
#include "blockitem.h"


Function::Function(std::string name)
    : name(name)
{
    ;
}

Function::Function(std::string name, std::vector<BlockItem*> items)
    : name(name), items(items)
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
    for (auto& x : items) {
        x->print_node(tabs + 1);
    }
}
