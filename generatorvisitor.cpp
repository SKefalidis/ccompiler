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
    if (expr->value) {
        expr->value->accept(this);
        std::string constant = results.top();
        results.pop();
        output << "\tmovl\t" << "$" << constant << ", %eax" << std::endl;
    } else {
        expr->expr->accept(this);
        expr->unary_op->accept(this);
//        std::string inner_expr = results.top();
//        results.pop();
//        std::string unary = results.top();
//        results.pop();
//        results.push(unary + inner_expr);
    }
}

void GeneratorVisitor::visit(Function* func)
{
    output << " .globl " << func->name << std::endl;
    output << func->name << ":" << std::endl;
    func->stm->accept(this);
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
    output << "\tret" << std::endl;
}

void GeneratorVisitor::visit(UnaryOperator* op)
{
//    results.push(op->op.value);
    switch (op->op.type) {
    case (TokenType::COMPLEMENT):
        output << "\tnot \t%eax" << std::endl;
        break;
    case (TokenType::MINUS):
        output << "\tneg \t%eax" << std::endl;
        break;
    case (TokenType::NEGATION):
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        output << "\tsete \t%al" << std::endl;
        break;
    default:
        std::cerr << "Unexpected TokenType" << std::endl;
    }
}
