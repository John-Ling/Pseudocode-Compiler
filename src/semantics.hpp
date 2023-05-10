#ifndef Semantics_Hpp
#define Semantics_Hpp

// Performs semantic analysis to detect semantic errors (i.e type mismatches, return statements outside functions, undeclared variables etc) before passing the nodes to the code generator

#include <iostream>
#include <vector>
#include <unordered_map>

#include "ast_nodes.hpp"
#include "token.hpp"
#include "constants.hpp"

class Semantic_Analyser
{
    private:
        typedef void (Semantic_Analyser::*nodeFunctionPointer)(Node*);

        // Symbol tables stores declarations (i.e name is mapped to type [INTEGER]) but expressions use literals (i.e 323 is [INTEGER_LITERAL]) this converts it for the comparison
        const std::unordered_map<std::string, std::string> LITERAL_TO_PRIMITIVE = { 
            {Tokens::STRING_LITERAL, Tokens::STRING}, {Tokens::BOOLEAN_LITERAL, Tokens::BOOLEAN},
            {Tokens::INTEGER_LITERAL, Tokens::INTEGER}, {Tokens::FLOAT_LITERAL, Tokens::FLOAT}};
        std::vector<Node*> nodes;

        std::unordered_map<std::string, std::string> symbolTable; // hash table that stores all identifiers and function names along with their type
        std::string analyserState;
        void examine(Node* node);
        void set_state(std::string state);
        std::string typecheck_expression(Node* node);
        Node* determine_expression_type(std::vector<std::string> types);
        void variable_assignment(Node* node);
        void variable_declaration(Node* node);
        void function(Node* node);
        void return_(Node* node);
    public:
        Semantic_Analyser(std::vector<Node*> tokens, std::unordered_map<std::string, std::string> symbolTable);
        int semantic_analysis(void);
};

namespace Analyser_States // The analyser can be in different states depending on what type of "code block" it is in such as function, while, if etc
{
    const std::string FUNCTION = "function";
    const std::string WHILE = "while";
    const std::string IF = "if";
    const std::string ELSE = "else";
    const std::string FOR = "for";
}

#endif