#include "exceptions.h"

void Lexical_Error::display_problem(std::string errorLine)
{
    std::cout << "Lexical Error: Error performing lexical analysis on line " << errorLine << '\n';
    return;
}

void Parse_Error::display_problem(std::string errorLine)
{
    std::cout << "Parsing Error: Unable to parse line " << errorLine << '\n';
    return;
}

void Type_Error::display_problem(std::string errorTokenType)
{
    std::cout << "Type Error Invalid type " << errorTokenType << " used\n";
    return;
}