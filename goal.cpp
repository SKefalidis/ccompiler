#include "goal.h"

Goal::Goal(Function* f)
{
    func = f;
}

void Goal::accept(Visitor* v)
{
    v->visit(this);
}
