#include "instruction.h"

Instruction::Instruction(std::string instr, std::string a1, std::string a2)
    : instruction(instr), a1(a1), a2(a2)
{
    ;
}

std::string Instruction::to_string()
{
    if (!a2.empty())
        return " \t" + instruction + " \t" + a1 + ", " + a2;
    else if (!a1.empty())
        return " \t" + instruction + " \t" + a1;
    else if (!is_label())
        return " \t" + instruction;
    else
        return instruction;
}

bool Instruction::is_label()
{
    return instruction.back()  == ':';
}

bool Instruction::is_jump()
{
    return instruction == "jmp" || instruction == "jne" || instruction == "je" || instruction == "ret";
}
