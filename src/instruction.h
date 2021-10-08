#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>


class Instruction
{
public:
    Instruction(std::string instr, std::string a1 = "", std::string a2 = "");
    std::string to_string();
    bool is_label();
    bool is_jump();
    bool is_guaranteed_jump();

    void replace(std::string instr, std::string a1 = "", std::string a2 = "");

    std::string instruction { "" };
    std::string a1          { "" };
    std::string a2          { "" };
};

#endif // INSTRUCTION_H
