#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include "node.h"
#include "expression.h"
#include "visitor.h"

enum class Type {
    FOR,
    WHILE,
    DO,
    RETURN,
    BREAK,
    CONTINUE,
    EMPTY,
    OTHER,
};


class Statement : public Node
{
public:
    Statement(Expression* expr, bool ret);
    Statement(Expression* expr, Statement* if_stm, Statement* else_stm);
    Statement(std::vector<BlockItem*> block_items);

    Statement(Expression* e1, Expression* e2, Expression* e3, Statement* body);
    Statement(Declaration* d, Expression* e2, Expression* e3, Statement* body);
    Statement(Type type, Expression* e1, Statement* body);

    Statement(Type type);

    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Type stm_type   { Type::OTHER };

    Expression* expr                    { nullptr };
    Statement* if_stm                   { nullptr };
    Statement* else_stm                 { nullptr };
    std::vector<BlockItem*> block_items {};

    Declaration* d  { nullptr };
    Expression* e1  { nullptr };
    Expression* e2  { nullptr };
    Expression* e3  { nullptr };
    Statement* body { nullptr };
};

#endif // STATEMENT_H
