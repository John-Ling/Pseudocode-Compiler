#include "token.h"

Token::Token(void)
{
    this->type = "";
    this->value = "";
}

Token::Token(std::string type, std::string value)
{
    this->type = type;
    this->value = value;
}
