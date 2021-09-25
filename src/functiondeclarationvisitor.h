#ifndef FUNCTIONDECLARATIONVISITOR_H
#define FUNCTIONDECLARATIONVISITOR_H

#include "visitor.h"
#include <unordered_map>
#include <list>
#include <string>


class FunctionDeclarationVisitor : public Visitor
{
public:
    FunctionDeclarationVisitor();

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
    std::unordered_map<std::string, std::list<FunctionDeclaration*>> declared_functions {};
    std::list<std::string> global_variables {};
    bool in_const_expression { false };
};

#endif // FUNCTIONDECLARATIONVISITOR_H
