#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>

#include "token.h"

class Parser
{
    private:
        std::vector<struct Token> tokenBuffer;
    public:
        Parser(std::vector<struct Token> tokens);
};

#endif