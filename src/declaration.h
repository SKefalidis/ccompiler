#ifndef DECLARATION_H
#define DECLARATION_H

#include "node.h"
#include "visitor.h"


class Declaration : public Node
{
public:
    Declaration(std::string id, Type* type, Expression* expr = nullptr);
    void accept(Visitor *v) override;
    void print_node(int tabs) const override;

    Expression* expr    { nullptr };
    std::string id      { "" };
    Type*       type    { nullptr };
};

#endif // DECLARATION_H
