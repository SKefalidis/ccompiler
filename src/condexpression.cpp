#include "condexpression.h"

CondExpression::CondExpression(OrExpression* expr, Expression* if_expr, CondExpression* else_expr)
    : expr(expr), if_expr(if_expr), else_expr(else_expr), Node(NodeType::COND_EXPRESSION)
{
    ;
}

void CondExpression::accept(Visitor* v)
{
    v->visit(this);
}

void CondExpression::print_node(int tabs) const
{

}
