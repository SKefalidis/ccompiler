#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include "node.h"
#include "expression.h"
#include "visitor.h"


class Statement : public Node
{
public:
    Statement(Expression* expr, bool ret);
    Statement(Expression* expr, std::string id);
    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    Expression* expr;
    std::string id;
    bool ret;
};

#endif // STATEMENT_H
