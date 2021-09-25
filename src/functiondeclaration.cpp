#include <iostream>
#include "functiondeclaration.h"
#include "blockitem.h"


FunctionDeclaration::FunctionDeclaration(std::string name, Type* return_type, std::vector<std::pair<Type*, std::string> > parameters)
    : name(name), return_type(return_type), parameters(parameters), Node(NodeType::FUNCTION_DECLARATION)
{
    definition = false;
}

FunctionDeclaration::FunctionDeclaration(std::string name, Type* return_type, std::vector<std::pair<Type*, std::string> > parameters, std::vector<BlockItem*> items)
    : name(name), return_type(return_type), parameters(parameters), items(items), Node(NodeType::FUNCTION_DECLARATION)
{
    definition = true;
}

void FunctionDeclaration::accept(Visitor* v)
{
    v->visit(this);
}

void FunctionDeclaration::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "FUNCTION" << std::endl;
    std::cout << tabs_string(tabs) << name << std::endl;
    for (auto& x : items) {
        x->print_node(tabs + 1);
    }
}
