#include "literal.h"

Literal::Literal(std::string value) : value(value)
{

}

void Literal::accept(Visitor* v)
{
    v->visit(this);
}

IntLiteral::IntLiteral(std::string value) : Literal(value)
{

}

void* IntLiteral::evaluate() const
{

}
