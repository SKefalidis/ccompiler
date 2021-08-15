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
    } else {
        fact->inner_factor->accept(this);
        fact->unary_op->accept(this);
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

void GeneratorVisitor::visit(RelationalExpression* expr)
{
    generate_binary_op(expr->binary_op);
}

void GeneratorVisitor::visit(Statement* stm)
{
    stm->expr->accept(this);
    output << "\tret" << std::endl;
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

void GeneratorVisitor::generate_binary_op(BinaryExprOp* binary_op)
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
