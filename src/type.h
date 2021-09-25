#ifndef TYPE_H
#define TYPE_H

#include "node.h"
#include "visitor.h"


class Type : public Node
{
public:
    Type(std::string type);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    std::string type;
};

#endif // TYPE_H
