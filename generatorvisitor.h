#ifndef GENERATORVISITOR_H
#define GENERATORVISITOR_H

#include <fstream>
#include <stack>
#include <string>
#include "visitor.h"
#include "binaryop.h"


class GeneratorVisitor : public Visitor
{
public:
    GeneratorVisitor(std::ofstream& stream);

    void visit(AdditiveExpression*      expr)  override;
    void visit(AndExpression*           expr)  override;
    void visit(EqualityExpression*      expr)  override;
    void visit(Expression*              expr)  override;
    void visit(Factor*                  fact)  override;
    void visit(Function*                func)  override;
    void visit(Goal*                    goal)  override;
    void visit(Literal*                 lit)   override;
    void visit(RelationalExpression*    expr)  override;
    void visit(Statement*               stm)   override;
    void visit(Term*                    term)  override;
    void visit(UnaryOperator*           op)    override;

private:
    void generate_binary_op(BinaryAddExprOp* binary_op);
    void generate_binary_op(BinaryAndExprOp* binary_op);
    void generate_binary_op(BinaryEqExprOp* binary_op);
    void generate_binary_op(BinaryExprOp* binary_op);
    void generate_binary_op(BinaryRelExprOp* binary_op);
    void generate_binary_op(BinaryTermOp* binary_op);

    std::ofstream& output;
    std::stack<std::string> results;
};

#endif // GENERATORVISITOR_H
