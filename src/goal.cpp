#include <iostream>
#include "goal.h"

Goal::Goal(std::vector<Node*> d)
    : decls(d), Node(NodeType::GOAL)
{
    ;
}

void Goal::accept(Visitor* v)
{
    v->visit(this);
}

// TODO: Update print_node
void Goal::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "GOAL" << std::endl;
//    func->print_node(tabs + 1);
}
