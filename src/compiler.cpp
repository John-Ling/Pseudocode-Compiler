#include "compiler.h"

// branch test
// second branch test
Compiler::Compiler(std::string filename)
{
    this->sourceFile = filename;
}

int Compiler::compile(void)
{
    // Lexical Analysis
    Lexer lexer(this->sourceFile);
    std::cout << "Performing Lexical Analysis\n";
    if (lexer.generate_tokens() == 1)
    {
        return 1;
    }
    // Parsing
    Parser parser(lexer.tokens);
    std::cout << "Parsing and Building AST\n";
    if (parser.parse_tokens() == 1)
    {
        return 1;
    }
    return 0;
}
