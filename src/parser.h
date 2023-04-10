#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>
#include <stdexcept>

#include "token.h"
#include "constants.h"
#include "ast_nodes.h"

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
    Node expression(void);
    Node logical_comparison(void);
    Node equality(void);
    Node numerical_comparison(void);
    Node term(void);
    Node factor(void);
    Node unary(void);
    Node primary();
    int function_call_parameter(void);
    int primitive_literal(void);
    int primitive_type(void);

public:
    Parser(std::vector<Token> tokens);
    int parse_tokens(void);
    int parser_test(void);
};

#endif