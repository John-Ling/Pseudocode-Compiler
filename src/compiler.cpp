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

    // Semantic Analysis
    Semantic_Analyser semanticAnalyser(parser.nodes, parser.get_symbol_table());
    std::cout << "Performing Semantic Analysis\n";
    if (semanticAnalyser.semantic_analysis() == 1)
    {
        return 1;
    }

    // Code Generation
    Code_Generator generator(this->sourceFile, parser.nodes, parser.get_symbol_table());
    std::cout << "Building Code\n";
    if (generator.generate_code() == 1)
    {
        return 1;
    }
    
    return 0;
}
