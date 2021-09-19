#include "blockitem.h"

BlockItem::BlockItem(Statement* stm)
    : stm(stm), Node(NodeType::BLOCK_ITEM)
{
    ;
}

BlockItem::BlockItem(Declaration* decl)
    : decl(decl), Node(NodeType::BLOCK_ITEM)
{
    ;
}

void BlockItem::accept(Visitor* v)
{
    v->visit(this);
}

void BlockItem::print_node(int tabs) const
{

}
