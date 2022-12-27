#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>

#include "token.h"

class Parser
{
    private:
        std::vector<struct Token> tokenBuffer;
        int basic_parsing(void);
    public:
        Parser(std::vector<struct Token> tokens);
};

#endif