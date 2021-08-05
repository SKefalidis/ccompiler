#ifndef VISITOR_H
#define VISITOR_H

class Expression;
class Function;
class Goal;
class Literal;
class Statement;

class Visitor
{
public:
    virtual void visit(Expression*  expr) = 0;
    virtual void visit(Function*    func) = 0;
    virtual void visit(Goal*        goal) = 0;
    virtual void visit(Literal*     lit)  = 0;
    virtual void visit(Statement*   stm)  = 0;
};

#endif // VISITOR_H
