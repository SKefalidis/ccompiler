#include "statement.h"

Statement::Statement(Expression* expr) : expr(expr)
{

}

void Statement::accept(Visitor* v)
{
    v->visit(this);
}
