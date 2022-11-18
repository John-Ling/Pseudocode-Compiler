#include "compiler.h"

// branch test
// second branch test
Compiler::Compiler(std::string filename)
{
    this->targetFile = filename;
}

int Compiler::compile(void)
{
    Lexer lexer(this->targetFile);
    int lexingProcess = lexer.generate_tokens();
    for (int i = 0; i < lexer.tokens.size(); i++)
    {
        std::cout << (lexer.tokens[i]).type << " ";
        std::cout << (lexer.tokens[i]).value << std::endl;
    }
    return 0;
}
