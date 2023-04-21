#include "exceptions.h"

void Lexical_Error::display_problem(void)
{
    std::cout << "Lexical Error: Error performing lexical analysis on line " << this->errorLine << '\n';
    return;
}

void Unexpected_Parsing_Input::display_problem(void)
{
    std::cout << "Parsing Error: Expected " << this->expected << " but received " << this->received << '\n';
    return;
}

void Missed_Terminating_Token::display_problem(void)
{
    std::cout << "Missed Terminating Token Error: Expected " << this->expected << '\n';
    return;
}

void Type_Error::display_problem(std::string errorTokenType)
{
    std::cout << "Type Error Invalid type " << errorTokenType << " used\n";
    return;
}