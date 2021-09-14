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
    if (variable_map.front().find(decl->id) != variable_map.front().end()) {
        std::cerr << "Double declaration" << std::endl;
        exit(1);
    }
    variable_map.front().insert({decl->id, stack_index});
    stack_index -= 4;
    if (decl->expr) {
        decl->expr->accept(this);
        output << "\tpushl \t%eax" << std::endl;
        output << "\tmovl \t%eax, " << get_variable(decl->id) << "(%ebp)" << std::endl;
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
    } else { /* assignment operation */
        expr->expr->accept(this);
        switch (expr->op.type) {
        case TokenType::PLUS:
            output << "\taddl \t%eax, " << get_variable(expr->id) << "(%ebp)" << std::endl;
        case TokenType::MINUS:
            output << "\tsubl \t%eax, " << get_variable(expr->id) << "(%ebp)" << std::endl;
        case TokenType::STAR:
            output << "\timul \t%eax, " << get_variable(expr->id) << "(%ebp)" << std::endl;
            break;
        case TokenType::SLASH:
            /* %ecx holds the divisor */
            output << "\tmovl \t%eax, %ecx" << std::endl;
            /* %EDX:%EAX hold the previous value of the variable */
            output << "\tmovl \t " << get_variable(expr->id) << "(%ebp), %eax" << std::endl;
            output << "\tcdq" << std::endl;
            /* execute the division */
            output << "\tidivl \t%ecx" << std::endl;
            /* move the quotient to the memory location of the variable */
            output << "\tmovl \t%eax, " << get_variable(expr->id) << "(%ebp)" << std::endl;
            break;
        case TokenType::MODULO:
            /* %ecx holds the divisor */
            output << "\tmovl \t%eax, %ecx" << std::endl;
            /* %EDX:%EAX hold the previous value of the variable */
            output << "\tmovl \t " << get_variable(expr->id) << "(%ebp), %eax" << std::endl;
            output << "\tcdq" << std::endl;
            /* execute the division */
            output << "\tidivl \t%ecx" << std::endl;
            /* move the remainder to the memory location of the variable */
            output << "\tmovl \t%edx, " << get_variable(expr->id) << "(%ebp)" << std::endl;
            break;
        case TokenType::ASSIGN:
            output << "\tmovl \t%eax, " << get_variable(expr->id) << "(%ebp)" << std::endl;
            break;
        default:
            ;
        }
        /* make sure that %eax has the correct value in case it is used */
        output << "\tmovl \t " << get_variable(expr->id) << "(%ebp), %eax" << std::endl;
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
        output << "\tmovl \t" << get_variable(fact->variable) << "(%ebp), %eax" << std::endl;
    } else {
        fact->inner_factor->accept(this);
        fact->unary_op->accept(this);
    }
}

void GeneratorVisitor::visit(Function* func)
{
    variable_map.push_front(std::unordered_map<std::string, int>());
    stack_index = -4;
    output << func->name << ":" << std::endl;

    output << "\tpush \t%ebp" << std::endl;
    output << "\tmovl \t%esp, %ebp" << std::endl;

    for (auto& x : func->items) {
        x->accept(this);
    }

    output << "\tmovl \t$0, %eax" << std::endl;
    output << "\tmovl \t%ebp, %esp" << std::endl;
    output << "\tpop \t%ebp" << std::endl;
    output << "\tret" << std::endl;
    variable_map.pop_front();
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
    if (stm->block_items.size() > 0) { /* new block */
        variable_map.push_front(std::unordered_map<std::string, int>());
        for (auto& i : stm->block_items) {
            i->accept(this);
        }
        stack_index += variable_map.front().size() * 4;
        variable_map.pop_front();
    } else if (stm->stm_type == Type::RETURN) { /* return statement */
        stm->expr->accept(this);
        output << "\tmovl \t%ebp, %esp" << std::endl;
        output << "\tpop \t%ebp" << std::endl;
        output << "\tret" << std::endl;
    } else if (stm->stm_type == Type::EMPTY) { /* empty statement */
        ;
    } else if (stm->stm_type == Type::BREAK) { /* empty statement */
        if (break_label.empty()) {
            throw "Illegal break statement";
        }
        output << "\tjmp " << break_label << std::endl;
    } else if (stm->stm_type == Type::CONTINUE) { /* empty statement */
        if (continue_label.empty()) {
            throw "Illegal continue statement";
        }
        output << "\tjmp " << continue_label << std::endl;
    } else if (stm->stm_type == Type::WHILE) { /* while statement */
        std::string start_label = get_label();
        std::string end_label = get_label();
        continue_label = start_label;
        break_label = end_label;

        output << start_label << ":" << std::endl;
        stm->e1->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tje " << end_label << std::endl;
        stm->body->accept(this);
        output << "\tjmp " << start_label << std::endl;
        output << end_label << ":" << std::endl;

        break_label = "";
        continue_label = "";
    } else if (stm->stm_type == Type::DO) { /* do statement */
        std::string start_label = get_label();
        std::string end_label = get_label();
        continue_label = start_label;
        break_label = end_label;

        output << start_label << ":" << std::endl;
        stm->body->accept(this);
        stm->e1->accept(this);
        output << "\tcmpl \t$0, %eax" << std::endl;
        output << "\tje " << end_label << std::endl;
        output << "\tjmp " << start_label << std::endl;
        output << end_label << ":" << std::endl;

        break_label = "";
        continue_label = "";
    } else if (stm->stm_type == Type::FOR) { /* for statement */
        std::string start_label = get_label();
        std::string end_label = get_label();
        std::string continue_label = get_label();
        this->continue_label = continue_label;
        break_label = end_label;

        output << "# for" << std::endl;
        output << "# e1" << std::endl;
        if (stm->d) {
            variable_map.push_front(std::unordered_map<std::string, int>());
            stm->d->accept(this);
        } else if (stm->e1) {
            stm->e1->accept(this);
        }

        output << start_label << ":" << std::endl;
        output << "# e2" << std::endl;
        if (stm->e2) {
            stm->e2->accept(this);
            output << "\tcmpl \t$0, %eax" << std::endl;
            output << "\tje " << end_label << std::endl;
        }
        output << "# body" << std::endl;
        stm->body->accept(this);
        output << "# e3" << std::endl;
        output << continue_label << ":" << std::endl;
        if (stm->e3) {
            stm->e3->accept(this);
        }
        output << "\tjmp " << start_label << std::endl;
        output << end_label << ":" << std::endl;

        if (stm->d) {
            stack_index += 4;
            variable_map.pop_front();
        }

        break_label = "";
        continue_label = "";
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
        } else if (term->op.type == TokenType::MODULO) {
            output << "\tmovl \t%eax, %ecx" << std::endl;
            output << "\tpop \t%eax" << std::endl;
            output << "\tcdq" << std::endl;
            output << "\tidivl \t%ecx" << std::endl;
            output << "\tmovl \t%edx, %eax" << std::endl;
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
    case (TokenType::INCREMENT):
        output << "\taddl \t$1, %eax" << std::endl;
        output << "\tmovl \t%eax, " << get_variable(op->id) << "(%ebp)" << std::endl;
        break;
    case (TokenType::DECREMENT):
        output << "\tsubl \t$1, %eax" << std::endl;
        output << "\tmovl \t%eax, " << get_variable(op->id) << "(%ebp)" << std::endl;
        break;
    default:
        std::cerr << "Unexpected TokenType" << std::endl;
    }
}

std::string GeneratorVisitor::get_label()
{
    return "_label" + std::to_string(label_counter++);
}

int GeneratorVisitor::get_variable(std::string var_name)
{
    int result { 0 };
    bool found { false };
    for (auto& level : variable_map) {
        if (level.find(var_name) != variable_map.front().end()) {
            result = level.at(var_name);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Variable doesn't exist" << std::endl;
        throw "Error";
    }

    return result;
}
