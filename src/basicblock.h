#ifndef BASICBLOCK_H
#define BASICBLOCK_H

#include <vector>
#include <string>
#include "instruction.h"


class BasicBlock
{
public:
    BasicBlock();

    bool is_code_block();

    std::vector<Instruction*> instructions;
    std::vector<std::string> lines;
};

#endif // BASICBLOCK_H
