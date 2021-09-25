#include "declaration.h"

Declaration::Declaration(std::string id, Type* type, Expression* expr)
    : id(id), type(type), expr(expr), Node(NodeType::DECLARATION)
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
