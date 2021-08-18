#ifndef GENERATORVISITOR_H
#define GENERATORVISITOR_H

#include <fstream>
#include <stack>
#include <string>
#include <unordered_map>
#include "visitor.h"


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
    void visit(OrExpression*            expr)  override;
    void visit(Factor*                  fact)  override;
    void visit(Function*                func)  override;
    void visit(Goal*                    goal)  override;
    void visit(Literal*                 lit)   override;
    void visit(RelationalExpression*    expr)  override;
    void visit(Statement*               stm)   override;
    void visit(Term*                    term)  override;
    void visit(UnaryOperator*           op)    override;

private:
    std::string get_label();

    std::ofstream& output;
    std::stack<std::string> results;
    int label_counter { 0 };

    std::unordered_map<std::string, int> variable_map {};
    int stack_index;
};

#endif // GENERATORVISITOR_H
