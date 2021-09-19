#include "unaryoperator.h"

UnaryOperator::UnaryOperator(Token t)
    : op(t), Node(NodeType::UNARY_OPERATOR)
{
    ;
}

UnaryOperator::UnaryOperator(Token t, std::string id)
    : op(t), id(id), Node(NodeType::UNARY_OPERATOR)
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
