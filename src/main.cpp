#include <iostream>
#include <stdexcept>

#include "compiler.h"

// This compiler will take the A Level Computer Science language "psuedocode" and translate to python then run the generated python file with the interpreter
// Author: John Ling

// Stages of Development
// Lexer: Done
// Parser: Done
// Code generation: Done
// Extra thing: Semantic Analysis + Extra pseudocode features such as arrays, file handling etc

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Run compiler with a file path as the only argument" << std::endl;
        return 1;
    }
    std::string filename = argv[1];

    Compiler compiler(filename);
    if (compiler.compile() == 0)
    {
        std::cout << "Compilation Successful" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Compilation Failed" << std::endl;
        return 1;
    }
}