#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include "node.h"
#include "expression.h"
#include "visitor.h"


class Statement : public Node
{
public:
    Statement(Expression* expr, bool ret);
    Statement(Expression* expr, Statement* if_stm, Statement* else_stm);
    Statement(std::vector<BlockItem*> block_items);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Expression* expr                    { nullptr };
    Statement* if_stm                   { nullptr };
    Statement* else_stm                 { nullptr };
    bool ret                            { false };
    std::vector<BlockItem*> block_items {};
};

#endif // STATEMENT_H
