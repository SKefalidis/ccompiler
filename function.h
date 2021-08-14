#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include "node.h"
#include "statement.h"


class Function : public Node
{
public:
    Function(std::string name, Statement* stm);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Statement* stm;
    std::string name;
};

#endif // FUNCTION_H
