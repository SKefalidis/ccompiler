#ifndef VISITOR_H
#define VISITOR_H

class Node;
class AdditiveExpression;
class AndExpression;
class EqualityExpression;
class Expression;
class OrExpression;
class Factor;
class Function;
class Goal;
class Literal;
class RelationalExpression;
class Statement;
class Term;
class UnaryOperator;

class Visitor
{
public:
    virtual void visit(AdditiveExpression*      expr) = 0;
    virtual void visit(AndExpression*           expr) = 0;
    virtual void visit(EqualityExpression*      expr) = 0;
    virtual void visit(Expression*              expr) = 0;
    virtual void visit(OrExpression*            expr) = 0;
    virtual void visit(Factor*                  fact) = 0;
    virtual void visit(Function*                func) = 0;
    virtual void visit(Goal*                    goal) = 0;
    virtual void visit(Literal*                 lit)  = 0;
    virtual void visit(RelationalExpression*    expr) = 0;
    virtual void visit(Statement*               stm)  = 0;
    virtual void visit(Term*                    term) = 0;
    virtual void visit(UnaryOperator*           op)   = 0;
};

#endif // VISITOR_H
