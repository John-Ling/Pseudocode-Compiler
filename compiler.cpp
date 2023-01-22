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

    // Parsing
    // Parser parser(lexer.tokens);
    Parser parser(lexer.tokens);
	parser.parse_tokens();
    return 0;
}
