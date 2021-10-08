#ifndef CONTROLFLOWGRAPH_H
#define CONTROLFLOWGRAPH_H

#include "basicblock.h"
#include <map>


struct CfgBlock
{
    CfgBlock(BasicBlock* b) : block(b) {}

    BasicBlock* block;
    std::vector<CfgBlock*> incoming;
};

class ControlFlowGraph
{
public:
    ControlFlowGraph(std::vector<BasicBlock*> blocks);

    std::vector<BasicBlock*> remove_dead_blocks();
private:
    std::vector<BasicBlock*> blocks;
    std::vector<std::pair<int, CfgBlock*>> cfg;
};

#endif // CONTROLFLOWGRAPH_H
