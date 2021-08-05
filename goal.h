#ifndef GOAL_H
#define GOAL_H

#include "function.h"
#include "visitor.h"


class Goal
{
public:
    Goal(Function* f);
    void accept(Visitor* v);

    Function* func;
};

#endif // GOAL_H
