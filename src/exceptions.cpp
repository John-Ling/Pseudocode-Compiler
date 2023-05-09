#include "exceptions.hpp"

std::string Error::get_type(void)
{
    return this->TYPE;
}

void Error::set_type(std::string type)
{
    this->TYPE = type;
}

Generic_Error::Generic_Error(std::string errorMessage)
{
    set_type(Exception_Types::GENERIC);
    this->errorMessage = errorMessage;
}

void Generic_Error::display_problem(void)
{
    std::cout << "Generic Error: " << this->errorMessage << '\n';
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

Type_Mismatch::Type_Mismatch(std::string expectedType, std::string receivedType)
{
    this->expectedType = expectedType;
    this->receivedType = receivedType;
}

void Type_Mismatch::display_problem(void)
{
    std::cout << "Semantic Error: Expression of type " << this->receivedType << " does not match variable of type " << this->expectedType << '\n';
    return;
}

Undeclared_Variable::Undeclared_Variable(std::string variableName)
{
    this->variableName = variableName;
}

void Undeclared_Variable::display_problem(void)
{
    std::cout << "Semantic Error: Variable " << this->variableName << " has not been declared.\n";
    return;
}

Improper_Keyword_Placement::Improper_Keyword_Placement(std::string keyword)
{
    this->keyword = keyword;
}

void Improper_Keyword_Placement::display_problem(void)
{
    std::cout << "Semantic Error: Keyword " << this->keyword << " is in improper location\n";
    return;
}

Missing_Keyword::Missing_Keyword(std::string keyword)
{
    this->keyword = keyword;
}

void Missing_Keyword::display_problem(void)
{
    std::cout << "Semantic Error: " << this->keyword << " is missing\n";
    return;
}

Redeclaration::Redeclaration(std::string variableName)
{
    this->variableName = variableName;
}

void Redeclaration::display_problem(void)
{
    std::cout << "Semantic Error: Redeclaration of variable " << this->variableName << '\n';
    return;
}