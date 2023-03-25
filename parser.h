#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>
#include <stdexcept>

#include "token.h"
#include "constants.h"

class Parser
{
private:
    std::vector<Token> tokens;
    std::vector<Token> currentLine;
    int pointer;
    void parse_line(int pointer);
    int statement(void);
    bool match(std::string tokenType);
    void advance(void);
    void backtrack(void);
    bool peek(std::string tokenType);
    int function(void);
    int function_parameter(void);
    int output(void);
    int input(void);
    int while_(void);
    int if_(void);
    int else_(void);
    int for_(void);
    int return_(void);
    int variable_declaration(void);
    int variable_assignment(void);
    int expression(void);
    int logical_comparison(void);
    int equality(void);
    int numerical_comparison(void);
    int term(void);
    int factor(void);
    int unary(void);
    int primary();
    int function_call_parameter(void);
    int primitive_literal(void);
    int primitive_type(void);

public:
    Parser(std::vector<Token> tokens);
    int parse_tokens(void);
    int parser_test(void);
};

#endif