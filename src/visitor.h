#ifndef VISITOR_H
#define VISITOR_H

class Node;
class AdditiveExpression;
class AndExpression;
class BlockItem;
class CondExpression;
class Declaration;
class EqualityExpression;
class Expression;
class ExpressionOptional;
class OrExpression;
class Factor;
class FunctionDeclaration;
class FunctionCall;
class Goal;
class Literal;
class RelationalExpression;
class Statement;
class Term;
class Type;
class UnaryOperator;

class Visitor
{
public:
    virtual void visit(AdditiveExpression*      expr) = 0;
    virtual void visit(AndExpression*           expr) = 0;
    virtual void visit(BlockItem*               item) = 0;
    virtual void visit(CondExpression*          expr) = 0;
    virtual void visit(Declaration*             decl) = 0;
    virtual void visit(EqualityExpression*      expr) = 0;
    virtual void visit(Expression*              expr) = 0;
    virtual void visit(ExpressionOptional*      expr) = 0;
    virtual void visit(OrExpression*            expr) = 0;
    virtual void visit(Factor*                  fact) = 0;
    virtual void visit(FunctionDeclaration*     func) = 0;
    virtual void visit(FunctionCall*            func) = 0;
    virtual void visit(Goal*                    goal) = 0;
    virtual void visit(Literal*                 lit)  = 0;
    virtual void visit(RelationalExpression*    expr) = 0;
    virtual void visit(Statement*               stm)  = 0;
    virtual void visit(Term*                    term) = 0;
    virtual void visit(Type*                    type);
    virtual void visit(UnaryOperator*           op)   = 0;
};

#endif // VISITOR_H
