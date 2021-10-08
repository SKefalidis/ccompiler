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

        /* remove block if it has no label and the previous block is a guaranteed jump */
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
                            continue;
                        }
                    }
                }
            }

            /* redundant push/pop */
            if (instrA->instruction == "push" || instrA->instruction == "pushl") {
                Instruction* instrB { nullptr };
                int j;
                for (j = i + 1; j < b->instructions.size(); j++) {
                    instrB = b->instructions.at(j);
                    if (instrB->instruction == "pop") {
                        break;
                    }

                    if (instrB->instruction == "push" || instrB->instruction == "pushl" || instrB->instruction == "call") {
                        break;
                    }
                }

                /* if you found the matching pop instruction */
                if (instrB && instrB->instruction == "pop") {
                    bool register_unused { true };
                    for (int k = i + 1; k < j; k++) {
                        auto instrC = b->instructions.at(k);
                        if (instrC->a1 == instrB->a1 || instrC->a2 == instrB->a1) {
                            register_unused = false;
                            break;
                        }
                    }


                    if (register_unused) {
                        instrA->replace("mov", instrA->a1, instrB->a1);
                        b->instructions.erase(b->instructions.begin() + j);
                        i--;
                        delete instrB;
                        continue;
                    }
                }
            }
        }
    }

    return blocks;
}
