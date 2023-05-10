#ifndef Code_Generator_Hpp
#define Code_Generator_Hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "token.hpp"
#include "ast_nodes.hpp"
#include "constants.hpp"

class Code_Generator
{
    private:
        typedef std::string (Code_Generator::*nodeFunctionPointer)(Node*);
        std::string targetFilename;
        std::vector<Node*> nodes;
        std::string indentation = ""; // Determines indentation of code. Usually used in block_statement().
        std::unordered_map<std::string, std::string> identifierTable; // maps identifiers to their types for type checking
        std::unordered_map<std::string, nodeFunctionPointer> nodeMap = {
        {AST_Node_Names::FUNCTION, &Code_Generator::function},
        {AST_Node_Names::WHILE, &Code_Generator::while_}, {AST_Node_Names::IF, &Code_Generator::if_}, {AST_Node_Names::FOR, &Code_Generator::for_},
        {AST_Node_Names::RETURN, &Code_Generator::return_}, {AST_Node_Names::LITERAL, &Code_Generator::literal},
        {AST_Node_Names::IDENTIFIER, &Code_Generator::identifier}, {AST_Node_Names::DECLARATION, &Code_Generator::variable_declaration}, 
        {AST_Node_Names::ASSIGNMENT, &Code_Generator::variable_assignment}, {AST_Node_Names::OUTPUT, &Code_Generator::output}, 
        {AST_Node_Names::INPUT, &Code_Generator::input}, {AST_Node_Names::BINARY_EXPRESSION, &Code_Generator::binary_expression}, {AST_Node_Names::BRACKETED_EXPRESSION, &Code_Generator::bracketed_expression},
        {AST_Node_Names::UNARY_EXPRESSION, &Code_Generator::unary_expression}, {AST_Node_Names::FUNCTION_CALL, &Code_Generator::function_call}, {AST_Node_Names::ARRAY_ASSIGNMENT, &Code_Generator::array_assignment},
        {AST_Node_Names::ARRAY_DECLARATION, &Code_Generator::array_declaration}, {AST_Node_Names::ARRAY_EXPRESSION, &Code_Generator::array_expression}};
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
        std::string primitive(std::string type);
        std::string variable_declaration(Node* node);
        std::string variable_assignment(Node* node);
        std::string output(Node* node);
        std::string input(Node* node);
        std::string array_expression(Node* node);
        std::string array_declaration(Node* node);
        std::string array_assignment(Node* node);
        std::string bracketed_expression(Node* node);
        std::string binary_expression(Node* node);
        std::string unary_expression(Node* node);
        std::string function_call(Node* node);
        std::string call_arguments(Node* node);
        std::string block_statement(std::vector<Node*> statements);
    public:
        Code_Generator(std::string filename, std::vector<Node*> nodes, std::unordered_map<std::string, std::string> identifierTable);
        int generate_code(void);
};

#endif