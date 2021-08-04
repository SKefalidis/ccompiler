#include "literal.h"

Literal::Literal(std::string value) : value(value)
{

}

IntLiteral::IntLiteral(std::string value) : Literal(value)
{

}
