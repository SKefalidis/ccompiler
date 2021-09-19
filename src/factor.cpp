#include <iostream>
#include "factor.h"
#include "expression.h"


Factor::Factor(Literal* literal)
    : value(literal), Node(NodeType::FACTOR)
{
    ;
}

Factor::Factor(UnaryOperator* op, Factor* factor)
    : unary_op(op), inner_factor(factor), Node(NodeType::FACTOR)
{
    ;
}

Factor::Factor(Expression* expr)
    : expr(expr), Node(NodeType::FACTOR)
{
    ;
}

Factor::Factor(std::string variable)
    : variable(variable), Node(NodeType::FACTOR)
{
    ;
}

Factor::Factor(FunctionCall* func)
    : func(func), Node(NodeType::FACTOR)
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
    } else if (!variable.empty()) {
        std::cout << tabs_string(tabs + 1) << variable << std::endl;
    }
}
