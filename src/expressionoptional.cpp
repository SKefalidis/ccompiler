#include "expressionoptional.h"

ExpressionOptional::ExpressionOptional(Expression* expr)
    : expr(expr), Node(NodeType::EXPRESSION_OPTIONAL)
{
    ;
}

void ExpressionOptional::accept(Visitor* v)
{
    v->visit(this);
}

void ExpressionOptional::print_node(int tabs) const
{
    ;
}

bool ExpressionOptional::is_empty()
{
    return expr == nullptr;
}
