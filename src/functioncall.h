#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include <string>
#include <vector>
#include "node.h"
#include "expression.h"


class FunctionCall : public Node
{
public:
    FunctionCall(std::string id, std::vector<Expression*> arguments);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    std::string id;
    std::vector<Expression*> arguments;
};

#endif // FUNCTIONCALL_H
