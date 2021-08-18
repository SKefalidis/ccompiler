#include <iostream>
#include "statement.h"

Statement::Statement(Expression* expr, bool ret)
    : expr(expr), ret(ret)
{
    ;
}

Statement::Statement(Expression* expr, Statement* if_stm, Statement* else_stm)
    : expr(expr), if_stm(if_stm), else_stm(else_stm)
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
    else
        std::cout << tabs_string(tabs) << "other" << std::endl;
    if (expr)
        expr->print_node(tabs + 1);
}
