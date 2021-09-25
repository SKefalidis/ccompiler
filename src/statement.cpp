#include <iostream>
#include "statement.h"

Statement::Statement(Expression* expr, bool ret)
    : expr(expr), Node(NodeType::STATEMENT)
{
    if (!expr->cond_expr && !expr->expr && expr->id.empty() && expr->op.type == TokenType::INVALID)
        stm_type = StatementType::EMPTY;
    else
        stm_type = ret ? StatementType::RETURN : StatementType::OTHER;
}

Statement::Statement(ExpressionOptional* expr)
    : expr_optional(expr), Node(NodeType::STATEMENT)
{
    ;
}

Statement::Statement(Expression* expr, Statement* if_stm, Statement* else_stm)
    : expr(expr), if_stm(if_stm), else_stm(else_stm), Node(NodeType::STATEMENT)
{
    ;
}

Statement::Statement(std::vector<BlockItem*> block_items)
    : block_items(block_items), Node(NodeType::STATEMENT)
{
    ;
}

Statement::Statement(ExpressionOptional* fe1, ExpressionOptional* fe2, ExpressionOptional* fe3, Statement* body)
    : for_e1(fe1), for_e2(fe2), for_e3(fe3), body(body), Node(NodeType::STATEMENT)
{
    stm_type = StatementType::FOR;
}

Statement::Statement(Declaration* d, ExpressionOptional* fe2, ExpressionOptional* fe3, Statement* body)
    : d(d), for_e2(fe2), for_e3(fe3), body(body), Node(NodeType::STATEMENT)
{
    stm_type = StatementType::FOR;
}

Statement::Statement(StatementType type, Expression* we, Statement* body)
    : stm_type(type), while_e(we), body(body), Node(NodeType::STATEMENT)
{
    ;
}

Statement::Statement(StatementType type)
    : stm_type(type), Node(NodeType::STATEMENT)
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
    if (stm_type == StatementType::RETURN)
        std::cout << tabs_string(tabs) << "return" << std::endl;
    else
        std::cout << tabs_string(tabs) << "other" << std::endl;
    if (expr)
        expr->print_node(tabs + 1);
}
