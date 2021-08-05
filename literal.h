#ifndef LITERAL_H
#define LITERAL_H

#include <string>
#include "visitor.h"


class Literal
{
public:
    Literal(std::string value);
    void accept(Visitor* v);
    virtual void* evaluate() const = 0;

    std::string value;
};

class IntLiteral : public Literal
{
public:
    IntLiteral(std::string value);
    void* evaluate() const override;
};

#endif // LITERAL_H
