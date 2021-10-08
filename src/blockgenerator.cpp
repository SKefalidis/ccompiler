#include <iostream>
#include "blockgenerator.h"


BlockGenerator::BlockGenerator(std::ifstream& file)
    : file(file)
{
    ;
}

std::vector<BasicBlock*> BlockGenerator::get_blocks()
{
    std::vector<BasicBlock*> blocks;
    BasicBlock* block { nullptr };

    if (!file.is_open()) {
        std::cerr << "Must open file to create basic blocks!" << std::endl;
        return blocks;
    }

    for (std::string line; std::getline(file, line);)
    {
        /* split line into components */
        std::vector<std::string> strings;
        strings.push_back("");
        for (auto& c : line) {
            if (c == ' ' || c == '\t' || c == '\n' || c == ',') {
                if (!strings.back().empty()) {
                    strings.push_back("");
                }
                continue;
            }

            strings.back() += c;
        }

        if (strings.size() == 0) {
            continue;
        }

        /* skip non-code segments */
        if (strings.at(0).at(0) == '.') {
            block = new BasicBlock;
            blocks.push_back(block);
            block->lines.push_back(line);
            continue;
        }

        /* create instruction */
        Instruction* instruction = new Instruction(strings.at(0), strings.size() > 1 ? strings.at(1) : "", strings.size() > 2 ? strings.at(2) : "");

        /* there is always at least a label */
        if (instruction->is_label()) {
            block = new BasicBlock;
            blocks.push_back(block);
        }

        block->instructions.push_back(instruction);

        if (instruction->is_jump()) {
            block = new BasicBlock;
            blocks.push_back(block);
        }
    }

    return blocks;
}
