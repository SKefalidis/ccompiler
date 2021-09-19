#include <iostream>
#include <string>
#include "generatorvisitor.h"
#include "additiveexpression.h"
#include "blockitem.h"
#include "condexpression.h"
#include "declaration.h"
#include "functiondeclaration.h"
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
        print_instr("push", "%eax");
        expr->term->accept(this);
        print_instr("pop", "%ecx");
        if (expr->op.type == TokenType::PLUS) {
            print_instr("addl", "%ecx", "%eax");
        } else if (expr->op.type == TokenType::MINUS) {
            print_instr("subl", "%eax", "%ecx");
            print_instr("movl", "%ecx", "%eax");
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
        print_instr("cmpl", "$0", "%eax");
        print_instr("jne", label);
        print_instr("jmp", end_label);
        output << label << ":" << std::endl;
        expr->expr->accept(this);
        print_instr("cmpl", "$0", "%eax");
        print_instr("movl", "$0", "%eax");
        print_instr("setne", "%al");
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
        print_instr("cmpl", "$0", "%eax");
        print_instr("je", label);
        expr->if_expr->accept(this);
        print_instr("jmp", end_label);
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
    variable_map.front().insert({decl->id, VariableInfo(std::to_string(stack_index), true)});
    stack_index -= 4;
    if (decl->expr) {
        decl->expr->accept(this);
        print_instr("pushl", "%eax");
        print_instr("movl", "%eax", get_variable_address(decl->id));
    }
}

void GeneratorVisitor::visit(EqualityExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        print_instr("push", "%eax");
        expr->expr->accept(this);
        print_instr("pop", "%ecx");
        print_instr("cmpl", "%eax", "%ecx");
        print_instr("movl", "$0", "%eax");
        switch (expr->op.type) {
        case (TokenType::EQ):
            print_instr("sete", "%al");
            break;
        case (TokenType::NEQ):
            print_instr("setne", "%al");
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
            print_instr("addl", "%eax", get_variable_address(expr->id));
        case TokenType::MINUS:
            print_instr("subl", "%eax", get_variable_address(expr->id));
        case TokenType::STAR:
            print_instr("imul", "%eax", get_variable_address(expr->id));
            break;
        case TokenType::SLASH:
            /* %ecx holds the divisor */
            print_instr("movl", "%eax", "%ecx");
            /* %EDX:%EAX hold the previous value of the variable */
            print_instr("movl", get_variable_address(expr->id), "%eax");
            print_instr("cdq");
            /* execute the division */
            print_instr("idivl", "%ecx");
            /* move the quotient to the memory location of the variable */
            print_instr("movl", "%eax", get_variable_address(expr->id));
            break;
        case TokenType::MODULO:
            /* %ecx holds the divisor */
            print_instr("movl", "%eax", "%ecx");
            /* %EDX:%EAX hold the previous value of the variable */
            print_instr("movl", get_variable_address(expr->id), "%eax");
            print_instr("cdq");
            /* execute the division */
            print_instr("idivl", "%ecx");
            /* move the remainder to the memory location of the variable */
            print_instr("movl", "%edx", get_variable_address(expr->id));
            break;
        case TokenType::INVALID:
            print_instr("movl", "%eax", get_variable_address(expr->id));
            break;
        default:
            ;
        }
        /* make sure that %eax has the correct value in case it is used */
        print_instr("movl", get_variable_address(expr->id), "%eax");
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
        print_instr("cmpl", "$0", "%eax");
        print_instr("je", label);
        print_instr("movl", "$1", "%eax");
        print_instr("jmp", end_label);
        output << label << ":" << std::endl;
        expr->expr->accept(this);
        print_instr("cmpl", "$0", "%eax");
        print_instr("movl", "$0", "%eax");
        print_instr("setne", "%al");
        output << end_label << ":" << std::endl;
    }
}

void GeneratorVisitor::visit(Factor* fact)
{
    if (fact->value) {
        fact->value->accept(this);
        std::string constant = results.top();
        results.pop();
        print_instr("movl", "$" + constant, "%eax");
    } else if (fact->expr) {
        fact->expr->accept(this);
    } else if (!fact->variable.empty()) {
        print_instr("movl", get_variable_address(fact->variable), "%eax");
    } else if (fact->func) {
        fact->func->accept(this);
    } else {
        fact->inner_factor->accept(this);
        fact->unary_op->accept(this);
    }
}

void GeneratorVisitor::visit(FunctionDeclaration* func)
{
    if (!func->definition) {
        return;
    }
    variable_map.push_front(std::unordered_map<std::string, VariableInfo>());
    stack_index = -4;

    /* parameters are placed above %ebp */
    int parameter_offset { 8 };
    for (auto& p : func->parameters) {
        variable_map.front().insert({p.second, VariableInfo(std::to_string(parameter_offset), true)});
        parameter_offset += 4;
    }

    output << func->name << ":" << std::endl;

    print_instr("push", "%ebp");
    print_instr("movl", "%esp", "%ebp");

    for (auto& x : func->items) {
        x->accept(this);
    }

    print_instr("movl", "$0", "%eax");
    print_instr("movl", "%ebp", "%esp");
    print_instr("pop", "%ebp");
    print_instr("ret");

    variable_map.pop_front();
}

void GeneratorVisitor::visit(FunctionCall* func)
{
    for (int i = func->arguments.size() - 1; i >= 0; i--) {
        func->arguments.at(i)->accept(this);
        print_instr("pushl", "%eax");
    }
    print_instr("call", func->id);
    print_instr("addl", "$" + std::to_string(func->arguments.size() * 4), "%esp");
}

void GeneratorVisitor::visit(Goal* goal)
{
    variable_map.push_front(std::unordered_map<std::string, VariableInfo>());

    for (auto& x : goal->decls) {
        if (x->type == NodeType::FUNCTION_DECLARATION) {
            output << " .globl " << static_cast<FunctionDeclaration*>(x)->name << std::endl;
            print_line(" .text");
            x->accept(this);
        } else { /* global variable */
            Declaration* decl = static_cast<Declaration*>(x);
            std::string label;

            /* declare */
            if (variable_map.front().find(decl->id) != variable_map.front().end()) {
                label = variable_map.front().at(decl->id).address;
            } else {
                label = get_label();
                variable_map.front().insert({decl->id, VariableInfo(label, false)});
            }

            /* define */
            if (decl->expr) {
                if (variable_map.front().at(decl->id).defined) {
                    std::cerr << "Global variable redefinition" << std::endl;
                    throw "Error";
                } else {
                    variable_map.front().at(decl->id).defined = true;
                }
                print_line(" .globl " + label);
                print_line(" .data");
                print_line(" .align 4");
                print_line(label + ":");
                print_line(" .long " +  decl->expr->cond_expr->expr->expr->expr->expr->expr->term->fact->value->value); /* instead of crashing it should report an error */
            }
        }
    }

    for (auto& var : variable_map.front()) {
        if (!var.second.defined) {
            std::string label = var.second.address;
            print_line(" .globl " + label);
            print_line(" .bss");
            print_line(" .align 4");
            print_line(label + ":");
            print_line(" .zero 4");
        }
    }

    variable_map.pop_front();
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
        print_instr("push", "%eax");
        expr->expr->accept(this);
        print_instr("pop", "%ecx");
        print_instr("cmpl", "%eax", "%ecx");
        print_instr("movl", "$0", "%eax");
        switch (expr->op.type) {
        case (TokenType::GE):
            print_instr("setge", "%al");
            break;
        case (TokenType::GT):
            print_instr("setg", "%al");
            break;
        case (TokenType::LE):
            print_instr("setle", "%al");
            break;
        case (TokenType::LT):
            print_instr("setl", "%al");
            break;
        default:
             std::cerr << "Unexpected TokenType" << std::endl;
        }
    }
}

void GeneratorVisitor::visit(Statement* stm)
{
    if (stm->block_items.size() > 0) { /* new block */
        variable_map.push_front(std::unordered_map<std::string, VariableInfo>());
        for (auto& i : stm->block_items) {
            i->accept(this);
        }
        stack_index += variable_map.front().size() * 4;
        variable_map.pop_front();
    } else if (stm->stm_type == Type::RETURN) { /* return statement */
        stm->expr->accept(this);
        print_instr("movl", "%ebp", "%esp");
        print_instr("pop", "%ebp");
        print_instr("ret");
    } else if (stm->stm_type == Type::EMPTY) { /* empty statement */
        ;
    } else if (stm->stm_type == Type::BREAK) { /* empty statement */
        if (break_label.empty()) {
            throw "Illegal break statement";
        }
        print_instr("jmp", break_label);
    } else if (stm->stm_type == Type::CONTINUE) { /* empty statement */
        if (continue_label.empty()) {
            throw "Illegal continue statement";
        }
        print_instr("jmp", continue_label);
    } else if (stm->stm_type == Type::WHILE) { /* while statement */
        std::string start_label = get_label();
        std::string end_label = get_label();
        continue_label = start_label;
        break_label = end_label;

        output << start_label << ":" << std::endl;
        stm->e1->accept(this);
        print_instr("cmpl", "$0", "%eax");
        print_instr("je", end_label);
        stm->body->accept(this);
        print_instr("jmp", start_label);
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
        print_instr("cmpl", "$0", "%eax");
        print_instr("je", end_label);
        print_instr("jmp", start_label);
        output << end_label << ":" << std::endl;

        break_label = "";
        continue_label = "";
    } else if (stm->stm_type == Type::FOR) { /* for statement */
        std::string start_label = get_label();
        std::string end_label = get_label();
        std::string continue_label = get_label();
        this->continue_label = continue_label;
        break_label = end_label;

        print_line("# for");
        print_line("# e1");
        if (stm->d) {
            variable_map.push_front(std::unordered_map<std::string, VariableInfo>());
            stm->d->accept(this);
        } else if (stm->e1) {
            stm->e1->accept(this);
        }

        output << start_label << ":" << std::endl;
        print_line("# e2");
        if (stm->e2) {
            stm->e2->accept(this);
            print_instr("cmpl", "$0", "%eax");
            print_instr("je", end_label);
        }
        stm->body->accept(this);
        print_line("# e3");
        output << continue_label << ":" << std::endl;
        if (stm->e3) {
            stm->e3->accept(this);
        }
        print_instr("jmp", start_label);
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
        print_instr("cmpl", "$0", "%eax");
        if (stm->else_stm) {
            print_instr("je", label);
            stm->if_stm->accept(this);
            print_instr("jmp", end_label);
            output << label << ":" << std::endl;
            stm->else_stm->accept(this);
        } else {
            print_instr("je", end_label);
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
        print_instr("push", "%eax");
        term->fact->accept(this);
        if (term->op.type == TokenType::STAR) {
            print_instr("pop", "%ecx");
            print_instr("imul", "%ecx", "%eax");
        } else if (term->op.type == TokenType::SLASH) {
            print_instr("movl", "%eax", "%ecx");
            print_instr("pop", "%eax");
            print_instr("cdq");
            print_instr("idivl", "%ecx");
        } else if (term->op.type == TokenType::MODULO) {
            print_instr("movl", "%eax", "%ecx");
            print_instr("pop", "%eax");
            print_instr("cdq");
            print_instr("idivl", "%ecx");
            print_instr("movl", "%edx", "%eax");
        }
    }
}

void GeneratorVisitor::visit(UnaryOperator* op)
{
    switch (op->op.type) {
    case (TokenType::COMPLEMENT):
        print_instr("not", "%eax");
        break;
    case (TokenType::MINUS):
        print_instr("neg", "%eax");
        break;
    case (TokenType::NEGATION):
        print_instr("cmpl", "$0", "%eax");
        print_instr("movl", "$0", "%eax");
        print_instr("sete", "%al");
        break;
    case (TokenType::INCREMENT):
        print_instr("addl", "$1", "%eax");
        print_instr("movl", "%eax", get_variable_address(op->id));
        break;
    case (TokenType::DECREMENT):
        print_instr("subl", "$1", "%eax");
        print_instr("movl", "%eax", get_variable_address(op->id));
                break;
    default:
        std::cerr << "Unexpected TokenType" << std::endl;
    }
}

//---------------------------------------------------------
//   utility functions
//---------------------------------------------------------

std::string GeneratorVisitor::get_label()
{
    return "_label" + std::to_string(label_counter++);
}

VariableInfo GeneratorVisitor::get_variable_info(std::string var_name)
{
    VariableInfo result("", false, false);
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

std::string GeneratorVisitor::get_variable_address(std::string id)
{
    VariableInfo info = get_variable_info(id);

    if (info.is_offset) {
        return info.address + "(%ebp)";
    } else {
        return info.address;
    }
}

void GeneratorVisitor::print_line(std::string line)
{
    output << line << std::endl;
}

void GeneratorVisitor::print_instr(std::string instruction)
{
    print_line(instruction);
}

void GeneratorVisitor::print_instr(std::string instruction, std::string arg1)
{
    print_line("\t" + instruction + " \t" + arg1);
}

void GeneratorVisitor::print_instr(std::string instruction, std::string arg1, std::string arg2)
{
    print_line("\t" + instruction + " \t" + arg1 + ", " + arg2);
}
