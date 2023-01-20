#include "parser.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->tokenPointer = 0;
}

int Parser::parse_tokens(void)
{
    for (int i = 0; i < this->tokens.size(); i++)
    {
        if (this->tokens[i].type != "[EOL]")
        {
            this->currentLine.push_back(this->tokens[i]);
        }
        else
        {
            parse_line(this->currentLine);
            this->currentLine.clear();
        }
    }
    return 0;
}

void Parser::parse_line(std::vector<Token> buffer)
{
    int status = statement(buffer[this->tokenPointer]); // attempt to parse line 
    return;
}

void Parser::advance(void)
{
    if (this->tokenPointer + 1 <= (this->currentLine.size() - 1))
    {
        this->tokenPointer++;
    }
    else
    {
        this->tokenPointer = -1;
    }
    return;
}

int Parser::statement(int pointer)
{
    Token token = this->currentLine[pointer];
    if (token.type == "[FUNCTION]")
    {
        function(pointer);
    }
    else if (token.type == "[WHILE]")
    {
        while_(token);
    }
    else if (token.type == "[OUTPUT]")
    {

    }
}

int Parser::function(int pointer)
{
    
    return 0;
}

int Parser::output(int pointer)
{

    return 0;
}

int Parser::expression(int pointer)
{
    if (primitive_literals(pointer) == 0 || binary_expression(pointer) == 0)
    return 0;
}

int Parser::primitive_literals(int pointer)
{
    if (this->currentLine[pointer].type == "[INTEGER_LITERAL]" ||
        this->currentLine[pointer].type == "[FLOAT_LITERAL]" ||
        this->currentLine[pointer].type == "[STRING_LITERAL]" || 
        this->currentLine[pointer].type == "[BOOLEAN_LITERAL]")
    {
        return 0;        
    }
    return 1;
}

int Parser::operator_(int pointer)
{
    if (this->currentLine[pointer].type == "[ADDITION]" || 
        this->currentLine[pointer].type == "[SUBTRACTION]" ||
        this->currentLine[pointer].type == "[MULTIPLICATION]" ||
        this->currentLine[pointer].type == "[DIVISION]" || 
        this->currentLine[pointer].type == "[MODULUS]")
    {
        return 0;
    }
    return 1;
}

int while_(Token token)
{
    return 0;
}

int Parser::binary_expression(int pointer)
{
    if (expression(pointer) == 0 && operator_(pointer + 1) == 0 && expression(pointer + 2) == 0)
    {
        return 0;
    }
    return 1;
}

int Parser::unary_expression(int pointer)
{
    if (this->currentLine[pointer].type == "[NOT]" || this->currentLine[pointer].type ==  && expression(pointer + 1) == 0)
    {
        return 0;
    }
    return 1;
}
