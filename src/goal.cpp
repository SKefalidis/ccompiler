#include <iostream>
#include "goal.h"

Goal::Goal(Function* f)
{
    func = f;
}

void Goal::accept(Visitor* v)
{
    v->visit(this);
}

void Goal::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "GOAL" << std::endl;
    func->print_node(tabs + 1);
}
