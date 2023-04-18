#ifndef Parser_H
#define Parser_H

#include <vector>
#include <iostream>
#include <stdexcept>

#include "token.h"
#include "constants.h"
#include "ast_nodes.h"
#include "exceptions.h"

class Parser
{
    private:
        std::vector<Token> tokens;
        std::vector<Token> currentLine;
        int pointer;
        void parse_line(int pointer);
        bool match(std::string tokenType);
        void advance(void);
        bool peek(std::string tokenType);
        Node* statement(void);
        int function(void);
        int function_parameter(void);
        int output(void);
        int input(void);
        int while_(void);
        int if_(void);
        int else_(void);
        Node* for_(void);
        Node* return_(void);
        Node* variable_declaration(void);
        Node* variable_assignment(void);
        Node* expression(void);
        Node* logical_comparison(void);
        Node* equality(void);
        Node* numerical_comparison(void);
        Node* term(void);
        Node* factor(void);
        Node* unary(void);
        Node* primary(void);
        int function_call_parameter(void);
        Node* primitive_literal(void);
        Node* primitive_type(void);

    public:
        std::vector<Node*> nodes; // abstract syntax tree 
        Parser(std::vector<Token> tokens);
        int parse_tokens(void);
        int parser_test(void);
};

#endif