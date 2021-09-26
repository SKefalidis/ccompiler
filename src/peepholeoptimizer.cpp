#include "peepholeoptimizer.h"

PeepholeOptimizer::PeepholeOptimizer()
{

}

std::vector<BasicBlock*> PeepholeOptimizer::optimize(std::vector<BasicBlock*> blocks)
{
    for (auto& b : blocks) {
        if (!b->is_code_block()) {
            continue;
        }
        for (int i = 0; i < b->instructions.size(); i++) {
            Instruction* instrA = b->instructions.at(i);
            if (instrA->instruction == "movl") {
                if (i + 1 < b->instructions.size()) {
                    Instruction* instrB = b->instructions.at(i + 1);
                    if (instrB->instruction == "movl") {
                        if (instrA->a1 == instrB->a2 && instrA->a2 == instrB->a1) {
                            b->instructions.erase(b->instructions.begin() + i + 1);
                            i--;
                            delete instrB;
                        }
                    }
                }
            }
        }
    }

    return blocks;
}
