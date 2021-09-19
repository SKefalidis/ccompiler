#include "functioncall.h"

FunctionCall::FunctionCall(std::string id, std::vector<Expression*> arguments)
    : id(id), arguments(arguments), Node(NodeType::FUNCTION_CALL)
{
    ;
}

void FunctionCall::accept(Visitor* v)
{
    v->visit(this);
}

void FunctionCall::print_node(int tabs) const
{
    ;
}
