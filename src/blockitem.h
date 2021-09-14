#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include "node.h"
#include "visitor.h"


class BlockItem : public Node
{
public:
    BlockItem(Statement* stm);
    BlockItem(Declaration* decl);
    void accept(Visitor *v) override;
    void print_node(int tabs) const override;

    Statement* stm      { nullptr };
    Declaration* decl   { nullptr };
};

#endif // BLOCKITEM_H
