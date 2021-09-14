#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include "node.h"


class Function : public Node
{
public:
    Function(std::string name);
    Function(std::string name, std::vector<BlockItem*> items);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    std::vector<BlockItem*> items {};
    std::string name { "" };
};

#endif // FUNCTION_H
