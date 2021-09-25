#include <iostream>
#include <stdexcept>

#include "functiondeclarationvisitor.h"
#include "additiveexpression.h"
#include "blockitem.h"
#include "condexpression.h"
#include "declaration.h"
#include "functiondeclaration.h"
#include "goal.h"
#include "andexpression.h"
#include "expressionoptional.h"
#include "equalityexpression.h"
#include "relationalexpression.h"
#include "statement.h"
#include "unaryoperator.h"
#include "literal.h"
#include "factor.h"
#include "term.h"

FunctionDeclarationVisitor::FunctionDeclarationVisitor()
{
    ;
}

void FunctionDeclarationVisitor::visit(AdditiveExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->term->accept(this);
    } else {
        expr->tail->accept(this);
        expr->term->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(AndExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        expr->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(BlockItem* item)
{
    if (item->decl) {
        item->decl->accept(this);
    } else if (item->stm) {
        item->stm->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(CondExpression* expr)
{
    if (expr->expr && !expr->if_expr) {
        expr->expr->accept(this);
    } else if (expr->expr && expr->if_expr && expr->else_expr) {
        expr->expr->accept(this);
        expr->if_expr->accept(this);
        expr->else_expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(Declaration* decl)
{
    if (decl->expr) {
        decl->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(EqualityExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        expr->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(Expression* expr)
{
    if (expr->cond_expr) {
        expr->cond_expr->accept(this);
    } else if (!expr->id.empty()) { /* assignment operation */
        expr->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(ExpressionOptional* expr)
{
    if (expr->expr) {
        expr->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(OrExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        expr->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(Factor* fact)
{
    if (fact->value) {
        fact->value->accept(this);
    } else if (fact->expr) {
        fact->expr->accept(this);
    } else if (!fact->variable.empty()) {
        if (in_const_expression) {
            std::cerr << "Cannot use variables in constant expression" << std::endl;
            throw "Constant expression error!";
        }
    } else if (fact->func) {
        fact->func->accept(this);
    } else {
        fact->inner_factor->accept(this);
        fact->unary_op->accept(this);
    }
}

// FIXME: Using exceptions for program flow isn't a great idea...
void FunctionDeclarationVisitor::visit(FunctionDeclaration* func)
{
    try {
        /* check argument count */
        auto declaration = declared_functions.at(func->name).front();
        if (func->parameters.size() != declaration->parameters.size()) {
            std::cerr << "Declaration of already declared function with different number of parameters" << std::endl;
            throw "Function declaration error!";
        }
        /* check for redefinition */
        if (func->definition) {
            for (auto& x : declared_functions.at(func->name)) {
                if (x->definition) {
                    std::cerr << "Function redefinition" << std::endl;
                    throw "Function redefinition error!";
                }
            }
        }
        /* accept function declaration/definition */
        declared_functions[func->name].push_front(func);
        for (auto& x : func->items) {
            x->accept(this);
        }
    }  catch (std::out_of_range) {
        /* accept function declaration/definition */
        declared_functions[func->name].push_front(func);
        for (auto& x : func->items) {
            x->accept(this);
        }
    }
}

void FunctionDeclarationVisitor::visit(FunctionCall* func)
{
    try {
        auto declaration = declared_functions.at(func->id).front();
        if (declaration->parameters.size() != func->arguments.size()) {
            std::cerr << "Incorrect number of arguments for function " << declaration->name << std::endl;
            throw "Function call error";
        }
        /* TODO: must also check arguments type */
    }  catch (std::out_of_range) {
        std::cerr << "Function: " << func->id << " has not been declared before being used" << std::endl;
        throw "Function call error";
    }
}

void FunctionDeclarationVisitor::visit(Goal* goal)
{
    for (auto& x : goal->decls) {
        if (x->type == NodeType::FUNCTION_DECLARATION) {
            x->accept(this);
        } else { /* global variable */
            Declaration* decl = static_cast<Declaration*>(x);
            global_variables.push_front(decl->id);

            /* define */
            if (decl->expr) {
                in_const_expression = true;
                decl->expr->accept(this);
                in_const_expression = false;
            }
        }
    }

    /* check for name collision between functions and global variables */
    for (auto& x : global_variables) {
        if (declared_functions.find(x) != declared_functions.end()) {
            std::cerr << "Function and global variable with the same name: " << x << std::endl;
            throw "Function/Variable definition error";
        }
    }
}

void FunctionDeclarationVisitor::visit(Literal* lit)
{
    ;
}

void FunctionDeclarationVisitor::visit(RelationalExpression* expr)
{
    if (expr->op.type == TokenType::INVALID) {
        expr->expr->accept(this);
    } else {
        expr->tail->accept(this);
        expr->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(Statement* stm)
{
    if (stm->block_items.size() > 0) { /* new block */
        for (auto& i : stm->block_items) {
            i->accept(this);
        }
    } else if (stm->stm_type == StatementType::RETURN) { /* return statement */
        stm->expr->accept(this);
    } else if (stm->stm_type == StatementType::EMPTY) { /* empty statement */
        ;
    } else if (stm->stm_type == StatementType::BREAK) { /* empty statement */
        ;
    } else if (stm->stm_type == StatementType::CONTINUE) { /* empty statement */
        ;
    } else if (stm->stm_type == StatementType::WHILE) { /* while statement */
        stm->we->accept(this);
        stm->body->accept(this);
    } else if (stm->stm_type == StatementType::DO) { /* do statement */
        stm->body->accept(this);
        stm->we->accept(this);
    } else if (stm->stm_type == StatementType::FOR) { /* for statement */
        if (stm->d) {
            stm->d->accept(this);
        } else if (stm->fe1) {
            stm->fe1->accept(this);
        }
        if (stm->fe2) {
            stm->fe2->accept(this);
        }
        stm->body->accept(this);
        if (stm->fe3) {
            stm->fe3->accept(this);
        }
    } else if (stm->expr && stm->if_stm) { /* if statement */
        stm->expr->accept(this);
        if (stm->else_stm) {
            stm->if_stm->accept(this);
            stm->else_stm->accept(this);
        } else {
            stm->if_stm->accept(this);
        }
    } else if (stm->expr) { /* exp; */
        stm->expr->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(Term* term)
{
    if (term->op.type == TokenType::INVALID) {
        term->fact->accept(this);
    } else {
        term->tail->accept(this);
        term->fact->accept(this);
    }
}

void FunctionDeclarationVisitor::visit(UnaryOperator* op)
{
    ;
}
