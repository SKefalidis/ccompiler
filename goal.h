#ifndef GOAL_H
#define GOAL_H

#include "node.h"
#include "function.h"
#include "visitor.h"


class Goal : public Node
{
public:
    Goal(Function* f);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Function* func { nullptr };
};

#endif // GOAL_H
