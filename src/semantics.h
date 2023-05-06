#ifndef Semantics_H
#define Semantics_H

// Performs semantic analysis to detect semantic errors (i.e type mismatches, return statements outside functions, undeclared variables etc) before passing the tokens to the code generator

#include <iostream>
#include <vector>
#include <unordered_map>

#include "ast_nodes.h"
#include "token.h"
#include "constants.h"

class Semantic_Analyser
{
    private:
        std::unordered_map<std::string, std::string> symbolTable; // hash table that stores all identifiers and function names along with their type
        std::vector<Node*> tokens;
    public:
        Semantic_Analyser(std::vector<Node*> tokens, std::unordered_map<std::string, std::string> symbolTable);
        int semantic_analysis(void);
};

#endif