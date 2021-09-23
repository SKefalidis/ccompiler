#include <iostream>
#include "statement.h"

Statement::Statement(Expression* expr, bool ret)
    : expr(expr), Node(NodeType::STATEMENT)
{
    if (!expr->cond_expr && !expr->expr && expr->id.empty() && expr->op.type == TokenType::INVALID)
        stm_type = Type::EMPTY;
    else
        stm_type = ret ? Type::RETURN : Type::OTHER;
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

Statement::Statement(Expression* e1, Expression* e2, Expression* e3, Statement* body)
    : e1(e1), e2(e2), e3(e3), body(body), Node(NodeType::STATEMENT)
{
    stm_type = Type::FOR;
}

Statement::Statement(Declaration* d, Expression* e2, Expression* e3, Statement* body)
    : d(d), e2(e2), e3(e3), body(body), Node(NodeType::STATEMENT)
{
    stm_type = Type::FOR;
}

Statement::Statement(Type type, Expression* e1, Statement* body)
    : stm_type(type), e1(e1), body(body), Node(NodeType::STATEMENT)
{
    ;
}

Statement::Statement(Type type)
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
    if (stm_type == Type::RETURN)
        std::cout << tabs_string(tabs) << "return" << std::endl;
    else
        std::cout << tabs_string(tabs) << "other" << std::endl;
    if (expr)
        expr->print_node(tabs + 1);
}
