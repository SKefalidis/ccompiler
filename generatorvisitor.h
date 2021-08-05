#ifndef GENERATORVISITOR_H
#define GENERATORVISITOR_H

#include <fstream>
#include <stack>
#include <string>
#include "visitor.h"


class GeneratorVisitor : public Visitor
{
public:
    GeneratorVisitor(std::ofstream& stream);

    void visit(Expression*  expr)  override;
    void visit(Function*    func)  override;
    void visit(Goal*        goal)  override;
    void visit(Literal*     lit)   override;
    void visit(Statement*   stm)   override;

private:
    std::ofstream& output;
    std::stack<std::string> results;
};

#endif // GENERATORVISITOR_H
