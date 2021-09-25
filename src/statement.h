#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include "node.h"
#include "expression.h"
#include "visitor.h"

enum class StatementType {
    FOR,
    WHILE,
    DO,
    RETURN,
    BREAK,
    CONTINUE,
    EMPTY,
    OTHER,
};


class Statement : public Node
{
public:
    Statement(Expression* expr, bool ret);
    Statement(ExpressionOptional* expr);
    Statement(Expression* expr, Statement* if_stm, Statement* else_stm);
    Statement(std::vector<BlockItem*> block_items);

    Statement(ExpressionOptional* fe1, ExpressionOptional* fe2, ExpressionOptional* fe3, Statement* body);
    Statement(Declaration* d, ExpressionOptional* fe2, ExpressionOptional* fe3, Statement* body);
    Statement(StatementType type, Expression* we, Statement* body);

    Statement(StatementType type);

    void accept(Visitor* v) override;
    void print_node(int tabs) const override;

    StatementType       stm_type        { StatementType::OTHER };

    Expression*         expr            { nullptr }; /* used by return statements and as the condition in if statements */
    ExpressionOptional* expr_optional   { nullptr }; /* used for normal statements */

    Declaration*        d               { nullptr };
    ExpressionOptional* for_e1          { nullptr };
    ExpressionOptional* for_e2          { nullptr };
    ExpressionOptional* for_e3          { nullptr };
    Expression*         while_e         { nullptr }; /* used as the condition in while and do loops */

    std::vector<BlockItem*> block_items {};
    Statement*          body            { nullptr };
    Statement*          if_stm          { nullptr };
    Statement*          else_stm        { nullptr };
};

#endif // STATEMENT_H
