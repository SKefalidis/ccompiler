#include <iostream>
#include "generatorvisitor.h"
#include "additiveexpression.h"
#include "blockitem.h"
#include "condexpression.h"
#include "declaration.h"
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
    if (expr->op.type == TokenType::INVALID) {
        expr->term->accept(this);
    } else {
        expr->tail->accept(this);
        output << "\tpush \t%eax" << std::endl;
        expr->term->accept(this);
        output << "\tpop \t%ecx" << std::endl;
        if (expr->op.type == TokenType::PLUS) {
            output << "\taddl \t%ecx, %eax" << std::endl;
        } else if (expr->op.type == TokenType::MINUS) {
            output << "\tsubl \t%eax, %ecx" << std::endl;
            output << "\tmovl \t%ecx, %eax" << std::endl;
        }
    }
}

void GeneratorVisitor::visit(AndExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        std::string label = get_label();
        std::string end_label = get_label();
        expr->tail->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tjne " << label << std::endl;
        output << "\tjmp " << end_label << std::endl;
        output << label << ":" << std::endl;
        expr->expr->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        output << "\tsetne \t%al" << std::endl;
        output << end_label << ":" << std::endl;
    }
}

void GeneratorVisitor::visit(BlockItem* item)
{
    if (item->decl) {
        item->decl->accept(this);
    } else if (item->stm) {
        item->stm->accept(this);
    }
}

void GeneratorVisitor::visit(CondExpression* expr)
{
    if (expr->expr && !expr->if_expr) {
        expr->expr->accept(this);
    } else if (expr->expr && expr->if_expr && expr->else_expr) {
        std::string label = get_label();
        std::string end_label = get_label();
        expr->expr->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tje " << label << std::endl;
        expr->if_expr->accept(this);
        output << "\tjmp " << end_label << std::endl;
        output << label << ":" << std::endl;
        expr->else_expr->accept(this);
        output << end_label << ":" << std::endl;
    }
}

void GeneratorVisitor::visit(Declaration* decl)
{
    if (variable_map.find(decl->id) != variable_map.end()) {
        std::cerr << "Double declaration" << std::endl;
        exit(1);
    }
    variable_map.insert({decl->id, stack_index});
    stack_index -= 4;
    if (decl->expr) {
        decl->expr->accept(this);
        output << "\tpushl \t%eax" << std::endl;
        output << "\tmovl \t%eax, " << variable_map.at(decl->id) << "(%ebp)" << std::endl;
    }
}

void GeneratorVisitor::visit(EqualityExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        output << "\tpush \t%eax" << std::endl;
        expr->expr->accept(this);
        output << "\tpop \t%ecx" << std::endl;
        output << "\tcmpl \t%eax, %ecx" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        switch (expr->op.type) {
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

void GeneratorVisitor::visit(Expression* expr)
{
    if (expr->cond_expr) {
        expr->cond_expr->accept(this);
    } else {
        expr->expr->accept(this);
        output << "\tmovl \t%eax, " << variable_map.at(expr->id) << "(%ebp)" << std::endl;
    }
}

void GeneratorVisitor::visit(OrExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        std::string label = get_label();
        std::string end_label = get_label();
        expr->tail->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tje " << label << std::endl;
        output << "\tmovl \t$1, %eax" << std::endl;
        output << "\tjmp " << end_label << std::endl;
        output << label << ":" << std::endl;
        expr->expr->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        output << "\tsetne \t%al" << std::endl;
        output << end_label << ":" << std::endl;
    }
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

    for (auto& x : func->items) {
        x->accept(this);
    }

    // if the last instruction isn't a return statement, return 0, FIXME: How to do this correctly?
//    if (func->items.empty() || !func->items.back()->stm || !func->items.back()->stm->ret) {
//        output << "\tmovl \t$0, %eax" << std::endl;
//    }

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
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        output << "\tpush \t%eax" << std::endl;
        expr->expr->accept(this);
        output << "\tpop \t%ecx" << std::endl;
        output << "\tcmpl \t%eax, %ecx" << std::endl;
        output << "\tmovl \t$0, %eax" << std::endl;
        switch (expr->op.type) {
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

void GeneratorVisitor::visit(Statement* stm)
{
    if (stm->ret) { /* return statement */
        stm->expr->accept(this);
    } else if (stm->expr && stm->if_stm) { /* if statement */
        std::string label = get_label();
        std::string end_label = get_label();
        stm->expr->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        if (stm->else_stm) {
            output << "\tje " << label << std::endl;
            stm->if_stm->accept(this);
            output << "\tjmp " << end_label << std::endl;
            output << label << ":" << std::endl;
            stm->else_stm->accept(this);
        } else {
            output << "\tje " << end_label << std::endl;
            stm->if_stm->accept(this);
        }
        output << end_label << ":" << std::endl;
    } else if (stm->expr) { /* exp; */
        stm->expr->accept(this);
    }
}

void GeneratorVisitor::visit(Term* term)
{
    if (term->op.type == TokenType::INVALID) {
        term->fact->accept(this);
    } else {
        term->tail->accept(this);
        output << "\tpush \t%eax" << std::endl;
        term->fact->accept(this);
        if (term->op.type == TokenType::STAR) {
            output << "\tpop \t%ecx" << std::endl;
            output << "\timul \t%ecx, %eax" << std::endl;
        } else if (term->op.type == TokenType::SLASH) {
            output << "\tmovl \t%eax, %ecx" << std::endl;
            output << "\tpop \t%eax" << std::endl;
            output << "\tcdq" << std::endl;
            output << "\tidivl \t%ecx" << std::endl;
        }
    }
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

std::string GeneratorVisitor::get_label()
{
    return "_label" + std::to_string(label_counter++);
}
