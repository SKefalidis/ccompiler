#ifndef LITERAL_H
#define LITERAL_H

#include <string>


class Literal
{
public:
    Literal(std::string value);

private:
    std::string value;
};

class IntLiteral : public Literal
{
public:
    IntLiteral(std::string value);
};

#endif // LITERAL_H
