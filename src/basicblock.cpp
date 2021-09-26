#include "basicblock.h"

BasicBlock::BasicBlock()
{

}

bool BasicBlock::is_code_block()
{
    return lines.size() == 0 && instructions.size() > 0;
}
