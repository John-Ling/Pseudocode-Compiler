#ifndef Token_H
#define Token_H

#include <string>

class Token
{
public:
    std::string type;
    std::string value;
    Token(void);
    Token(std::string type, std::string value);
};

#endif