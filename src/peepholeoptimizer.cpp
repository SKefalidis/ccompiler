#include "peepholeoptimizer.h"
#include <algorithm>

PeepholeOptimizer::PeepholeOptimizer()
{

}

std::vector<BasicBlock*> PeepholeOptimizer::optimize(std::vector<BasicBlock*> blocks)
{
    bool ends_with_return { false };

    for (int bi = 0; bi < blocks.size(); bi++) {
        auto b = blocks.at(bi);
        if (!b->is_code_block()) {
            continue;
        }

        if (!b->instructions.front()->is_label() && bi > 0) {
            auto pb = blocks.at(bi - 1);
            if (pb->is_code_block() && pb->instructions.back()->is_guaranteed_jump()) {
                blocks.erase(blocks.begin() + bi);
                bi--;
                delete b;
                continue;
            }
        }

        for (int i = 0; i < b->instructions.size(); i++) {
            Instruction* instrA = b->instructions.at(i);

            /* redundant load/store store/load */
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

            /* redundant push/pop */
//            if (instrA->instruction == "push") {
//                if (i + 1 < b->instructions.size()) {
//                    Instruction* instrB = b->instructions.at(i + 1);
//                    if (instrB->instruction == "pop") {
//                        instrA->replace("mov", instrA->a1, instrB->a1);
//                        b->instructions.erase(b->instructions.begin() + i + 1);
//                        i--;
//                        delete instrB;
//                    }
//                }
//            }
        }

//        if (b->instructions.back()->instruction == "ret") {
//            if (ends_with_return) {
//                auto iterator = std::find(blocks.begin(), blocks.end(), b);
//                blocks.erase(iterator);
//                goto rerun;
//            }
//            ends_with_return = true;
//        } else {
//            ends_with_return = false;
//        }
    }

    return blocks;
}
