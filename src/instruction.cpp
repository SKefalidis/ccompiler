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
    return instruction.back()  == ':' && instruction.at(1) != 'g';
}

bool Instruction::is_jump()
{
    return instruction == "jmp" || instruction == "jne" || instruction == "je" || instruction == "ret";
}

bool Instruction::is_guaranteed_jump()
{
    return instruction == "jmp" || instruction == "ret";
}

bool Instruction::is_move()
{
    return instruction == "mov" || instruction == "movl";
}

void Instruction::replace(std::string instr, std::string a1, std::string a2)
{
    instruction = instr;
    this->a1 = a1;
    this->a2 = a2;
}
