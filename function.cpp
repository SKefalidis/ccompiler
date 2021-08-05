#include "function.h"

Function::Function(std::string name, Statement* stm)
    : name(name), stm(stm)
{

}

void Function::accept(Visitor* v)
{
    v->visit(this);
}
