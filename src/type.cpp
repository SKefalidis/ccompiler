#include "type.h"

Type::Type(std::string type)
    : type(type), Node(NodeType::TYPE)
{
    ;
}

void Type::accept(Visitor* v)
{
    v->visit(this);
}

void Type::print_node(int tabs) const
{
    ;
}
