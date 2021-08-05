#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include "statement.h"


class Function
{
public:
    Function(std::string name, Statement* stm);
    void accept(Visitor* v);

    Statement* stm;
    std::string name;
};

#endif // FUNCTION_H
