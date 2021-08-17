#include <iostream>
#include "statement.h"

Statement::Statement(Expression* expr, bool ret)
    : expr(expr), ret(ret)
{
    ;
}

Statement::Statement(Expression* expr, std::string id)
    : expr(expr), id(id)
{
    ;
}

void Statement::accept(Visitor* v)
{
    v->visit(this);
}

void Statement::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "STATEMENT" << std::endl;
    if (ret)
        std::cout << tabs_string(tabs) << "return" << std::endl;
    else if (!id.empty() && expr)
        std::cout << tabs_string(tabs) << id << " = " << std::endl;
    else if (!id.empty())
        std::cout << tabs_string(tabs) << "declaration" << std::endl;
    else
        std::cout << tabs_string(tabs) << "other" << std::endl;
    if (expr)
        expr->print_node(tabs + 1);
}
