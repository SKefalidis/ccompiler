#include "declaration.h"

Declaration::Declaration(std::string id, Expression* expr)
    : id(id), expr(expr), Node(NodeType::DECLARATION)
{
    ;
}

void Declaration::accept(Visitor* v)
{
    v->visit(this);
}

void Declaration::print_node(int tabs) const
{

}
