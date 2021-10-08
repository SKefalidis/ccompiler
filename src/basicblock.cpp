#include "basicblock.h"

BasicBlock::BasicBlock()
{

}

bool BasicBlock::is_code_block()
{
    return lines.size() == 0 && instructions.size() > 0;
}

std::string BasicBlock::get_label()
{
    if (instructions.front()->is_label()) {
        return instructions.front()->instruction;
    } else {
        return "";
    }
}
