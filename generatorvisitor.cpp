#include <iostream>
#include "generatorvisitor.h"
#include "expression.h"
#include "function.h"
#include "goal.h"
#include "statement.h"

GeneratorVisitor::GeneratorVisitor(std::ofstream& stream) : output(stream)
{
    ;
}

void GeneratorVisitor::visit(Expression* expr)
{
    expr->value->accept(this);
}

void GeneratorVisitor::visit(Goal* goal)
{
    goal->func->accept(this);
}

void GeneratorVisitor::visit(Literal* lit)
{
    results.push(lit->value);
}

void GeneratorVisitor::visit(Statement* stm)
{
    stm->expr->accept(this);
    std::string constant = results.top();
    results.pop();
    output << "\tmovl\t" << "$" << constant << ", %eax" << std::endl;
    output << "\tret" << std::endl;
}

void GeneratorVisitor::visit(Function* func)
{
    output << " .globl " << func->name << std::endl;
    output << func->name << ":" << std::endl;
    func->stm->accept(this);
}
