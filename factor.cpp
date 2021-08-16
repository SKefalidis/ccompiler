#include <iostream>
#include "factor.h"

Factor::Factor(Literal* literal)
    : value(literal)
{
    ;
}

Factor::Factor(UnaryOperator* op, Factor* factor)
    : unary_op(op), inner_factor(factor)
{
    ;
}

Factor::Factor(Expression* expr)
    : expr(expr)
{
    ;
}

Factor::Factor(std::string variable)
    : variable(variable)
{
    ;
}

void Factor::accept(Visitor* v)
{
    v->visit(this);
}

void Factor::print_node(int tabs) const
{
    std::cout << tabs_string(tabs) << "FACTOR" << std::endl;
    if (value) {
        std::cout << tabs_string(tabs + 1) << value->value << std::endl;
    } else if (unary_op && inner_factor) {

    } else if (expr) {
        expr->print_node(tabs + 1);
    }
}
