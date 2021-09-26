#ifndef BLOCKGENERATOR_H
#define BLOCKGENERATOR_H

#include <fstream>
#include <vector>
#include "basicblock.h"


class BlockGenerator
{
public:
    BlockGenerator(std::ifstream& file);

    std::vector<BasicBlock*> get_blocks();

private:
    std::ifstream& file;
};

#endif // BLOCKGENERATOR_H
