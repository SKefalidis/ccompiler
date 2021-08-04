#ifndef GOAL_H
#define GOAL_H

#include "function.h"


class Goal
{
public:
    Goal(Function* f);

private:
    Function* func;
};

#endif // GOAL_H
