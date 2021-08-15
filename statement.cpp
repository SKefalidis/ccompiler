#include <iostream>
#include "statement.h"

Statement::Statement(AdditiveExpression* expr) : expr(expr)
{

}

void Statement::accept(Visitor* v)
{
    v->visit(this);
}

void Statement::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "STATEMENT" << std::endl;
    std::cout << tabs_string(tabs) << "return" << std::endl;
    expr->print_node(tabs + 1);
}
