#include <iostream>
#include "generatorvisitor.h"
#include "additiveexpression.h"
#include "function.h"
#include "goal.h"
#include "andexpression.h"
#include "equalityexpression.h"
#include "relationalexpression.h"
#include "statement.h"
#include "unaryoperator.h"
#include "literal.h"
#include "factor.h"
#include "term.h"


GeneratorVisitor::GeneratorVisitor(std::ofstream& stream) : output(stream)
{
    ;
}

void GeneratorVisitor::visit(AdditiveExpression* expr)
{
    generate_binary_op(expr->binary_op);
}

void GeneratorVisitor::visit(AndExpression* expr)
{
    generate_binary_op(expr->binary_op);
}

void GeneratorVisitor::visit(EqualityExpression* expr)
{
    generate_binary_op(expr->binary_op);
}

void GeneratorVisitor::visit(Expression* expr)
{
    if (expr->or_expr) {
        expr->or_expr->accept(this);
    } else {
        expr->expr->accept(this);
        output << "\tmovl \t%eax, " << variable_map.at(expr->id) << "(%ebp)" << std::endl;
    }
}

void GeneratorVisitor::visit(OrExpression* expr)
{
    generate_binary_op(expr->binary_op);
}

void GeneratorVisitor::visit(Factor* fact)
{
    if (fact->value) {
        fact->value->accept(this);
        std::string constant = results.top();
        results.pop();
        output << "\tmovl\t" << "$" << constant << ", %eax" << std::endl;
    } else if (fact->expr) {
        fact->expr->accept(this);
    } else if (!fact->variable.empty()) {
        output << "\tmovl \t" << variable_map.at(fact->variable) << "(%ebp), %eax" << std::endl;
    } else {
        fact->inner_factor->accept(this);
        fact->unary_op->accept(this);
    }
}

void GeneratorVisitor::visit(Function* func)
{
    stack_index = -4;
    output << func->name << ":" << std::endl;

    output << "\tpush \t%ebp" << std::endl;
    output << "\tmovl \t%esp, %ebp" << std::endl;

    for (auto& x : func->stm) {
        x->accept(this);
    }

    // if the last instruction isn't a return statement, return 0
    if (func->stm.empty() || !func->stm.back()->ret) {
        output << "\tmovl \t$0, %eax" << std::endl;
    }

    output << "\tmovl \t%ebp, %esp" << std::endl;
    output << "\tpop \t%ebp" << std::endl;
    output << "\tret" << std::endl;
}

void GeneratorVisitor::visit(Goal* goal)
{
    output << " .globl " << goal->func->name << std::endl;
    goal->func->accept(this);
}

void GeneratorVisitor::visit(Literal* lit)
{
    results.push(lit->value);
}

void GeneratorVisitor::visit(RelationalExpression* expr)
{
    generate_binary_op(expr->binary_op);
}

void GeneratorVisitor::visit(Statement* stm)
{
    if (stm->ret) { /* return statement */
        stm->expr->accept(this);
    } else if (stm->id.empty()) { /* exp; */
        stm->expr->accept(this);
    } else { /* declare (& define) variable */
        if (variable_map.find(stm->id) != variable_map.end()) {
            std::cerr << "Double declaration" << std::endl;
            exit(1);
        }
        variable_map.insert({stm->id, stack_index});
        stack_index -= 4;
        if (stm->expr) {
            stm->expr->accept(this);
            output << "\tpushl \t%eax" << std::endl;
            output << "\tmovl \t%eax, " << variable_map.at(stm->id) << "(%ebp)" << std::endl;
        }
    }
}

void GeneratorVisitor::visit(Term* term)
{
    generate_binary_op(term->binary_op);
}

void GeneratorVisitor::visit(UnaryOperator* op)
{
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

void GeneratorVisitor::generate_binary_op(BinaryAddExprOp* binary_op)
{
    if (binary_op->op.type == TokenType::INVALID) {
        binary_op->next_term->accept(this);
    } else {
        generate_binary_op(binary_op->term);
        output << "\tpush \t%eax" << std::endl;
        binary_op->next_term->accept(this);
        output << "\tpop \t%ecx" << std::endl;
        if (binary_op->op.type == TokenType::PLUS) {
            output << "\taddl \t%ecx, %eax" << std::endl;
        } else if (binary_op->op.type == TokenType::MINUS) {
            output << "\tsubl \t%eax, %ecx" << std::endl;
            output << "\tmovl \t%ecx, %eax" << std::endl;
        }
    }
}

void GeneratorVisitor::generate_binary_op(BinaryAndExprOp* binary_op)
{
    if (binary_op->op.type == TokenType::INVALID) {
        binary_op->next_expr->accept(this);
    } else {
        std::string label = get_label();
        std::string end_label = get_label();
        generate_binary_op(binary_op->expr);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tjne " << label << std::endl;
        output << "\tjmp " << end_label << std::endl;
        output << label << ":" << std::endl;
        binary_op->next_expr->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        output << "\tsetne \t%al" << std::endl;
        output << end_label << ":" << std::endl;
    }
}

void GeneratorVisitor::generate_binary_op(BinaryEqExprOp* binary_op)
{
    if (binary_op->op.type == TokenType::INVALID) {
        binary_op->next_expr->accept(this);
    } else {
        generate_binary_op(binary_op->expr);
        output << "\tpush \t%eax" << std::endl;
        binary_op->next_expr->accept(this);
        output << "\tpop \t%ecx" << std::endl;
        output << "\tcmpl \t%eax, %ecx" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        switch (binary_op->op.type) {
        case (TokenType::EQ):
            output << "\tsete \t%al" << std::endl;
            break;
        case (TokenType::NEQ):
            output << "\tsetne \t%al" << std::endl;
            break;
        default:
             std::cerr << "Unexpected TokenType" << std::endl;
        }
    }
}

void GeneratorVisitor::generate_binary_op(BinaryOrExprOp* binary_op)
{
    if (binary_op->op.type == TokenType::INVALID) {
        binary_op->next_expr->accept(this);
    } else {
        std::string label = get_label();
        std::string end_label = get_label();
        generate_binary_op(binary_op->expr);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tje " << label << std::endl;
        output << "\tmovl \t$1, %eax" << std::endl;
        output << "\tjmp " << end_label << std::endl;
        output << label << ":" << std::endl;
        binary_op->next_expr->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        output << "\tsetne \t%al" << std::endl;
        output << end_label << ":" << std::endl;
    }
}

void GeneratorVisitor::generate_binary_op(BinaryRelExprOp* binary_op)
{
    if (binary_op->op.type == TokenType::INVALID) {
        binary_op->next_expr->accept(this);
    } else {
        generate_binary_op(binary_op->expr);
        output << "\tpush \t%eax" << std::endl;
        binary_op->next_expr->accept(this);
        output << "\tpop \t%ecx" << std::endl;
        output << "\tcmpl \t%eax, %ecx" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        switch (binary_op->op.type) {
        case (TokenType::GE):
            output << "\tsetge \t%al" << std::endl;
            break;
        case (TokenType::GT):
            output << "\tsetg \t%al" << std::endl;
            break;
        case (TokenType::LE):
            output << "\tsetle \t%al" << std::endl;
            break;
        case (TokenType::LT):
            output << "\tsetl \t%al" << std::endl;
            break;
        default:
             std::cerr << "Unexpected TokenType" << std::endl;
        }
    }
}

void GeneratorVisitor::generate_binary_op(BinaryTermOp* binary_op)
{
    if (binary_op->op.type == TokenType::INVALID) {
        binary_op->next_fact->accept(this);
    } else {
        generate_binary_op(binary_op->fact);
        output << "\tpush \t%eax" << std::endl;
        binary_op->next_fact->accept(this);
        if (binary_op->op.type == TokenType::STAR) {
            output << "\tpop \t%ecx" << std::endl;
            output << "\timul \t%ecx, %eax" << std::endl;
        } else if (binary_op->op.type == TokenType::SLASH) {
            output << "\tmovl \t%eax, %ecx" << std::endl;
            output << "\tpop \t%eax" << std::endl;
            output << "\tcdq" << std::endl;
            output << "\tidivl \t%ecx" << std::endl;
        }
    }
}

std::string GeneratorVisitor::get_label()
{
    return "_label" + std::to_string(label_counter++);
}
