#include "exceptions.h"

std::string Error::get_type(void)
{
    return this->TYPE;
}

void Error::set_type(std::string type)
{
    this->TYPE = type;
}

Generic_Error::Generic_Error(void)
{
    set_type(Exception_Types::GENERIC);
}

void Generic_Error::display_problem(void)
{
    std::cout << "Generic Error: Good luck debugging this lol\n";
    return;
}

Lexical_Error::Lexical_Error(std::string errorLine)
{
    set_type(Exception_Types::LEXICAL);
    this->errorLine = errorLine;
}

void Lexical_Error::display_problem(void)
{
    std::cout << "Lexical Error: Could not perform lexical analysis on line " << this->errorLine << '\n';
    return;
}

Unexpected_Parsing_Input::Unexpected_Parsing_Input(std::string received, std::string expected)
{
    set_type(Exception_Types::UNEXPECTED_TOKEN);
    this->received = received;
    this->expected = expected;
}

void Unexpected_Parsing_Input::display_problem(void)
{
    std::cout << "Parsing Error: Expected token of type " << this->expected << " but received token of type " << this->received << '\n';
    return;
}

Missed_Terminating_Token::Missed_Terminating_Token(std::string expected)
{
    set_type(Exception_Types::MISSED_TERMINATOR);
    this->expected = expected;
}

void Missed_Terminating_Token::display_problem(void)
{
    std::cout << "Parsing Error: Missed terminating token. Expected token of type " << this->expected << '\n';
    return;
}

Reached_End_Of_File::Reached_End_Of_File(void) { set_type(Exception_Types::END); }
void Reached_End_Of_File::display_problem(void) { return; }