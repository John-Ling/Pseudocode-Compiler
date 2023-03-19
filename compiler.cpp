#include "compiler.h"

// branch test
// second branch test
Compiler::Compiler(std::string filename)
{
    this->targetFile = filename;
}

int Compiler::compile(void)
{
    // Lexing
    Lexer lexer(this->targetFile);
    int lexingProcess = lexer.generate_tokens();
    if (lexingProcess == 1)
    {
        return 1;
    }
    // Parsing
    Parser parser(lexer.tokens);
	int parsingProcess = parser.parse_tokens();
    if (parsingProcess == 1)
    {
        return 1;
    }
    return 0;
}
