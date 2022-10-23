#include <iostream>

#include "compiler.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Run this command with a single file as an argument." << std::endl;
        return 1;
    }
    std::string filename = argv[1];

    Compiler compiler(filename);
    std::cout << compiler.SYMBOLS << std::endl;
    return 0;
}