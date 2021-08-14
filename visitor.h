#ifndef VISITOR_H
#define VISITOR_H

class Node;
class Expression;
class Factor;
class Function;
class Goal;
class Literal;
class Statement;
class Term;
class UnaryOperator;

class Visitor
{
public:
    virtual void visit(Expression*      expr) = 0;
    virtual void visit(Factor*          fact) = 0;
    virtual void visit(Function*        func) = 0;
    virtual void visit(Goal*            goal) = 0;
    virtual void visit(Literal*         lit)  = 0;
    virtual void visit(Statement*       stm)  = 0;
    virtual void visit(Term*            term) = 0;
    virtual void visit(UnaryOperator*   op)   = 0;
};

#endif // VISITOR_H
