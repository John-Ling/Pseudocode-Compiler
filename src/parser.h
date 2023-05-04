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
        std::unordered_map<std::string, std::string> identifierTable; // Stores the identifiers along with their types in a program. Will be used for type checking in code generation.
        int pointer;
        void parse_line(void);
        bool compare(std::string tokenType);
        void match(std::string tokenType);
        void advance(void);
        void double_advance(void);
        bool peek(std::string tokenType);
        Node* statement(void);
        std::vector<Node*> block_statement(std::vector<std::string> terminators);
        Node* function(void);
        Node* function_arguments(void);
        Node* output(void);
        Node* input(void);
        Node* while_(void);
        Node* if_(void);
        Node* else_(void);
        Node* for_(void);
        Node* return_(void);
        Node* variable_declaration(void);
        Node* variable_assignment(void);
        Node* identifier_(void);
        Node* expression(void);
        Node* logical_comparison(void);
        Node* equality(void);
        Node* numerical_comparison(void);
        Node* term(void);
        Node* factor(void);
        Node* unary(void);
        Node* primary(void);
        Node* function_call_arguments(void);
        Node* primitive_literal(void);
        Node* primitive_type(void);
    public:
        std::vector<Node*> nodes; // abstract syntax tree 
        Parser(std::vector<Token> tokens);
        int parse_tokens(void);
        std::unordered_map<std::string, std::string> get_identifier_table(void);
};

#endif