#ifndef Token_H
#define Token_H

#include <string>

typedef struct Token_T
{
    std::string type;
    std::string value;

    Token_T(void) {}
    Token_T(std::string type, std::string value)
    {
        this->type = type;
        this->value = value;
    }
} Token;

#endif