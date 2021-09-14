#include "unaryoperator.h"

UnaryOperator::UnaryOperator(Token t)
    : op(t)
{
    ;
}

UnaryOperator::UnaryOperator(Token t, std::string id)
    : op(t), id(id)
{
    ;
}

void UnaryOperator::accept(Visitor* v)
{
    v->visit(this);
}

void UnaryOperator::print_node(int tabs) const
{

}
