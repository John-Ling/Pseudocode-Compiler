#ifndef Compiler_H
#define Compiler_H
#include <iostream>

#include "lexer.h"
#include "parser.h"
class Compiler
{    
    private:
        std::string targetFile;
    public:
        Compiler(std::string filename);
        int compile(void);
};

#endif