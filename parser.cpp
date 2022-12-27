#include "parser.h"

Parser::Parser(std::vector<struct Token> tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].type;
        std::cout << " " << tokens[i].value << std::endl;
    }
}

int Parser::basic_parsing(void)
{
    std::vector<struct Token> tokenStack = {{"[OUTPUT]", ""}, {"[STRING_LITERAL]", ""}};
    unsigned int pointer = 0;
    struct Token nextToken = tokenStack[pointer];
    unsigned int returnValue = 0;

    return returnValue;
}