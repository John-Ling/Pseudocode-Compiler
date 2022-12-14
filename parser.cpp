#include "parser.h"

Parser::Parser(std::vector<struct Token> tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].type;
        std::cout << " " << tokens[i].value << std::endl;
    }
}