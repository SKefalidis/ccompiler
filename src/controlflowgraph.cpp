#include "controlflowgraph.h"


ControlFlowGraph::ControlFlowGraph(std::vector<BasicBlock*> blocks)
{
    this->blocks = blocks;
    for (int i = 0; i < blocks.size(); i++) {
        auto b = blocks.at(i);
        if (b->is_code_block()) {
            cfg.push_back(std::pair<int, CfgBlock*>(i, new CfgBlock(b)));
        }
    }

    for (int i = 0; i < cfg.size(); i++) {
        auto cfg_block = cfg[i].second;
        if (cfg_block->block->instructions.back()->is_guaranteed_jump()) {
            // add incoming to the block with the matching label
            if (cfg_block->block->instructions.back()->instruction == "jmp") {
                std::string label_dest = cfg_block->block->instructions.back()->a1 + ":";
                for (auto& pair : cfg) {
                    if (pair.second->block->get_label() == label_dest) {
                        pair.second->incoming.push_back(cfg_block);
                    }
                }
            }
        } else if (cfg_block->block->instructions.back()->is_jump()) {
            // add incoming to the block with the matching label and to the next block
            std::string label_dest = cfg_block->block->instructions.back()->a1 + ":";
            for (auto& pair : cfg) {
                if (pair.second->block->get_label() == label_dest) {
                    pair.second->incoming.push_back(cfg_block);
                }
            }
            if (i + 1 < cfg.size()) {
                cfg[i + 1].second->incoming.push_back(cfg_block);
            }
        } else {
            // add incoming to the next block
            if (i + 1 < cfg.size()) {
                cfg[i + 1].second->incoming.push_back(cfg_block);
            }
        }
    }
}

std::vector<BasicBlock*> ControlFlowGraph::remove_dead_blocks()
{
    std::vector<BasicBlock*> return_blocks = blocks;
    int removed = 0;

    for (auto& pair : cfg) {
        if (!pair.second->block->get_label().empty() && pair.second->block->get_label().at(0) != '_') { /* ignore if this is the start of a function */
            continue;
        }
        if (pair.second->incoming.empty()) {
            return_blocks.erase(return_blocks.begin() + pair.first - removed);
            removed++;
        }
    }

    return return_blocks;
}
