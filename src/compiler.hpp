#ifndef Compiler_Hpp
#define Compiler_Hpp
#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "semantics.hpp"
#include "code_generator.hpp"

class Compiler
{    
    private:
        std::string sourceFile;
    public: 
        Compiler(std::string filename);
        int compile(void);
};

#endif