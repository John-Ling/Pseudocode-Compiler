#include "compiler.h"

Compiler::Compiler(std::string filename)
{
    targetFile = filename;
}

int Compiler::compile()
{
    int lexingProcess = begin_lexing();
    if (lexingProcess == 1) 
        return 1;
    
    return 0;
}

int Compiler::begin_lexing()
{
    std::ifstream fileReader(targetFile);
    if (!fileReader.is_open())
        return 1;
    
    while (fileReader)
    {
        std::string line;
        std::getline(fileReader, line);
        tokenize(line);
    }
    for (int i = 0; i < tokens.size(); i ++)
    {
        std::cout << tokens[i] << std::endl;
    }
    return 0;
}


void Compiler::tokenize(std::string line)
{
    // tokenize a line from the target file and generate a vector containing the tokens
    std::string buffer = "";
    for (int i = 0; i < line.length(); i++)
    {
        std::string c(1, line[i]);
        if (KEYWORDS_TO_TOKENS.count(c))
            tokens.push_back(KEYWORDS_TO_TOKENS.at(c));
        else if (c == " ")
        {
            // process buffer
            if (KEYWORDS_TO_TOKENS.count(buffer))
                tokens.push_back(KEYWORDS_TO_TOKENS.at(buffer));
            buffer = "";
        }
        buffer = buffer + c;
    }
}

