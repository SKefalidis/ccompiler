#ifndef GOAL_H
#define GOAL_H

#include <vector>
#include "node.h"
#include "functiondeclaration.h"
#include "visitor.h"


class Goal : public Node
{
public:
    Goal(std::vector<FunctionDeclaration*> f);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    std::vector<FunctionDeclaration*> func { nullptr };
};

#endif // GOAL_H
