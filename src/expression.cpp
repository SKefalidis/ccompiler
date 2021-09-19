#include <iostream>
#include "expression.h"
#include "condexpression.h"


Expression::Expression(CondExpression* cond_expr)
    : cond_expr(cond_expr), Node(NodeType::EXPRESSION)
{
    ;
}

Expression::Expression(std::string id, Expression* expr, Token op)
    : id(id), expr(expr), op(op), Node(NodeType::EXPRESSION)
{
    ;
}

void Expression::accept(Visitor* v)
{
    v->visit(this);
}

void Expression::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "EXPRESSION" << std::endl;
    if (cond_expr) {
        cond_expr->print_node(tabs + 1);
    } else {
        std::cout << tabs_string(tabs) << id << " = " << std::endl;
        expr->print_node(tabs + 1);
    }
}
