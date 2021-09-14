#include "declaration.h"

Declaration::Declaration(std::string id, Expression* expr)
    : id(id), expr(expr)
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
