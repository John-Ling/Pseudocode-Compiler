#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>

#include "token.h"

class Parser
{
    private:
        std::vector<Token> tokens;
        std::vector<Token> currentLine;
        int tokenPointer;
        void parse_line(std::vector<Token> currentLine, int pointer);
        int statement(int pointer);
        int function(int pointer);
        int output(int pointer);
        int expression(int pointer);
        int primitive_literals(int pointer);
        int operator_(int pointer);
        int binary_expression(int pointer);
        int unary_expression(int pointer);
        void advance(void);
    public:
        Parser(std::vector<Token> tokens);
        int parse_tokens(void);
};

#endif