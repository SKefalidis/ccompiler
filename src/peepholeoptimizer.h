#ifndef PEEPHOLEOPTIMIZER_H
#define PEEPHOLEOPTIMIZER_H

#include "basicblock.h"


class PeepholeOptimizer
{
public:
    PeepholeOptimizer();
    std::vector<BasicBlock*> optimize(std::vector<BasicBlock*> blocks);
};

#endif // PEEPHOLEOPTIMIZER_H
