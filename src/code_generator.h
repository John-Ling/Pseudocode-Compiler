#ifndef Code_Generator_H
#define Code_Generator_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "token.h"
#include "ast_nodes.h"
#include "constants.h"

class Code_Generator
{
    private:
        typedef std::string (Code_Generator::*nodeFunctionPointer)(Node*);
        std::string targetFilename;
        std::vector<Node* > nodes;
        int pointer;
        void advance(void);
        std::string examine(Node* node);
        std::string convert_operator(Token token);
        std::string function(Node* node);
        std::string function_arguments(Node* node);
        std::string while_(Node* node);
        std::string if_(Node* node);
        std::string else_(Node* node);
        std::string for_(Node* node);
        std::string return_(Node* node);
        std::string literal(Node* node);
        std::string identifier(Node* node);
        std::string primitive(Node* node);
        std::string variable_declaration(Node* node);
        std::string variable_assignment(Node* node);
        std::string output(Node* node);
        std::string input(Node* node);
        std::string binary_expression(Node* node);
        std::string unary_expression(Node* node);
        std::string function_call(Node* node);
        std::string call_arguments(Node* node);
        std::string block_statement(std::vector<Node*> statements);
    public:
        Code_Generator(std::string filename, std::vector<Node*> nodes);
        int generate_code(void);
};

#endif