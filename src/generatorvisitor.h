#ifndef GENERATORVISITOR_H
#define GENERATORVISITOR_H

#include <fstream>
#include <deque>
#include <stack>
#include <string>
#include <unordered_map>
#include "visitor.h"


struct VariableInfo {
    VariableInfo (std::string address, bool is_offset, bool defined = false) {
        this->address = address;
        this->defined = defined;
        this->is_offset = is_offset;
    }

    std::string address;
    bool defined;
    bool is_offset;
};

class GeneratorVisitor : public Visitor
{
public:
    GeneratorVisitor(std::ofstream& stream);

    void visit(AdditiveExpression*      expr)  override;
    void visit(AndExpression*           expr)  override;
    void visit(BlockItem*               item)  override;
    void visit(CondExpression*          expr)  override;
    void visit(Declaration*             decl)  override;
    void visit(EqualityExpression*      expr)  override;
    void visit(Expression*              expr)  override;
    void visit(ExpressionOptional*      expr)  override;
    void visit(OrExpression*            expr)  override;
    void visit(Factor*                  fact)  override;
    void visit(FunctionDeclaration*     func)  override;
    void visit(FunctionCall*            func)  override;
    void visit(Goal*                    goal)  override;
    void visit(Literal*                 lit)   override;
    void visit(RelationalExpression*    expr)  override;
    void visit(Statement*               stm)   override;
    void visit(Term*                    term)  override;
    void visit(UnaryOperator*           op)    override;

private:
    std::string get_label();
    VariableInfo get_variable_info(std::string var_name);
    std::string get_variable_address(std::string id);
    void print_line(std::string line);
    void print_instr(std::string instruction);
    void print_instr(std::string instruction, std::string arg1);
    void print_instr(std::string instruction, std::string arg1, std::string arg2);

    std::ofstream& output;
    std::stack<std::string> results;
    int label_counter { 0 };

    std::deque<std::unordered_map<std::string, VariableInfo>> variable_map {};
    int stack_index;

    std::string break_label     { "" };
    std::string continue_label  { "" };
};

#endif // GENERATORVISITOR_H
