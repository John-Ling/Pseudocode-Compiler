#ifndef Parser_H
#define Parser_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "token.h"
#include "ast_nodes.h"
#include "exceptions.h"

class Parser
{
    private:
        std::vector<Token> tokens;
        std::vector<Token> currentLine;
        int pointer;
        void parse_line(void);
        bool compare(std::string tokenType);
        void match(std::string tokenType);
        void advance(void);
        void double_advance(void);
        bool peek(std::string tokenType);
        Node statement(void);
        std::vector<Node> block_statement(std::vector<std::string> terminators);
        Function function(void);
        Function_Arguments function_arguments(void);
        Output output(void);
        Input input(void);
        While while_(void);
        If if_(void);
        Else else_(void);
        For for_(void);
        Return return_(void);
        Variable_Declaration variable_declaration(void);
        Variable_Assignment variable_assignment(void);
        Node expression(void);
        Node logical_comparison(void);
        Node equality(void);
        Node numerical_comparison(void);
        Node term(void);
        Node factor(void);
        Node unary(void);
        Node primary(void);
        Call_Arguments function_call_arguments(void);
        Literal primitive_literal(void);
        Primitive primitive_type(void);
    public:
        std::vector<Node> nodes; // abstract syntax tree 
        Parser(std::vector<Token> tokens);
        int parse_tokens(void);
        int get_pointer(void);
};

#endif