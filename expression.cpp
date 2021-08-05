#include "expression.h"

Expression::Expression(Literal* literal) : value(literal)
{

}

void Expression::accept(Visitor* v)
{
    v->visit(this);
}
