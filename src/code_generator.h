#ifndef Code_Generator_H
#define Code_Generator_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "ast_nodes.h"
#include "constants.h"

class Code_Generator
{
    private:
        typedef void (Code_Generator::*nodeFunctionPointer)(Node);
        std::string targetFilename;
        std::vector<Node> nodes;
        void examine(Node node);
        void function(Node node);
    public:
        Code_Generator(std::string filename, std::vector<Node> nodes);
        int generate_code(void);
};

#endif