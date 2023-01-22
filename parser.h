#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>
#include <stdexcept>

#include "token.h"

class Parser
{
    private:
        std::vector<Token> tokens;
        std::vector<Token> currentLine;
        int pointer;
        void parse_line(int pointer);
        int statement();
        int function();
		int function_parameter();
        int output();
        int expression(int pointer);
        int primitive_literal(void);
		int primitive_type(void);
        int operator_(int pointer);
        int binary_expression(int pointer);
        int unary_expression(int pointer);
        void advance(void);
		int variable_declaration(void);
    public:
        Parser(std::vector<Token> tokens);
        int parse_tokens(void);
		int parser_test(void);
};

#endif