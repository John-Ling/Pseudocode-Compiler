#include <iostream>

#include "compiler.h"

// TODO
// Implement lexer 

// Stages of Development
// Lexer: Current
// Parser: Not Done
// Code Generation: Not Done

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Run this command with a single file as an argument." << std::endl;
        return 1;
    }
    std::string filename = argv[1];

    Compiler compiler(filename);
    if (compiler.compile() == 0)
        std::cout << "Compilation Successful" << std::endl;
    else
        std::cout << "Compilation Failed" << std::endl;
    return 0;
}