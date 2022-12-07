#ifndef Compiler_H
#define Compiler_H
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include "lexer.h"

class Compiler
{    
    private:
        std::string targetFile;
    public:
        Compiler(std::string filename);
        int compile(void);
};

#endif