#include "blockitem.h"

BlockItem::BlockItem(Statement* stm)
    : stm(stm)
{
    ;
}

BlockItem::BlockItem(Declaration* decl)
    : decl(decl)
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
