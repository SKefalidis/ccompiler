#include "peepholeoptimizer.h"
#include <algorithm>
#include <iostream>



PeepholeOptimizer::PeepholeOptimizer()
{
    ;
}

std::vector<BasicBlock*> PeepholeOptimizer::optimize(std::vector<BasicBlock*> blocks)
{
    bool ends_with_return   { false };
    bool no_change          { true };

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
                no_change = false;
                continue;
            }
        }

        for (int i = 0; i < b->instructions.size(); i++) {
            Instruction* instrA = b->instructions.at(i);

            /* redundant load/store store/load */
            if (instrA->is_move()) {
                if (i + 1 < b->instructions.size()) {
                    Instruction* instrB = b->instructions.at(i + 1);
                    if (instrB->is_move()) {
                        if (instrA->a1 == instrB->a2 && instrA->a2 == instrB->a1) {
                            b->instructions.erase(b->instructions.begin() + i + 1);
                            i--;
                            delete instrB;
                            no_change = false;
                            continue;
                        }
                    }
                }
            }

            /* redundant push/pop
             * push 	%eax
             * movl 	$1, %eax
             * ...
             * not using %ecx
             * ...
             * pop  	%ecx
            */
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
                        no_change = false;
                        continue;
                    }
                }
            }

            /* redundant push/pop
             * 	push 	%eax
             *  movl 	$2, %eax
             *  movl 	%eax, %ecx
             *  pop 	%eax
             */
            if (instrA->instruction == "push" || instrA->instruction == "pushl") {
                if (i + 3 < b->instructions.size()) {
                    auto instrB = b->instructions.at(i + 1);
                    auto instrC = b->instructions.at(i + 2);
                    auto instrD = b->instructions.at(i + 3);

                    if (instrB->is_move() && instrB->a2 == instrA->a1
                    && instrC->is_move() && instrC->a1 == instrA->a1
                    && instrD->instruction == "pop" && instrC->a1 == instrA->a1) {
                        instrA->replace(instrB->instruction, instrB->a1, instrC->a2);

                        b->instructions.erase(b->instructions.begin() + i + 1);
                        b->instructions.erase(b->instructions.begin() + i + 1);
                        b->instructions.erase(b->instructions.begin() + i + 1);
                        delete instrB;
                        delete instrC;
                        delete instrD;

                        i--;
                        no_change = false;
                        continue;
                    }
                }
            }
        }
    }

    if (!no_change) {
//        std::cout << "Optimize again" << std::endl;
        return optimize(blocks);
    }

    return blocks;
}
