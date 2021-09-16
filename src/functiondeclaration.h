#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include "node.h"


class FunctionDeclaration : public Node
{
public:
    FunctionDeclaration(std::string name, std::vector<std::pair<std::string, std::string>> parameters);
    FunctionDeclaration(std::string name, std::vector<std::pair<std::string, std::string>> parameters, std::vector<BlockItem*> items);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    std::vector<BlockItem*> items {};
    std::vector<std::pair<std::string, std::string>> parameters {};
    std::string name { "" };
};

#endif // FUNCTION_H
