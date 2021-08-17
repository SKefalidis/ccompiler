#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include "node.h"
#include "statement.h"


class Function : public Node
{
public:
    Function(std::string name);
    Function(std::string name, std::vector<Statement*> stm);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    std::vector<Statement*> stm {};
    std::string name { "" };
};

#endif // FUNCTION_H
