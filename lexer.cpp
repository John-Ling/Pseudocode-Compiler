#include "lexer.h"

Lexer::Lexer(std::string filename)
{
    this->sourceFile = filename;
    this->position = -1; // index for characters in each line of file
}

int Lexer::generate_tokens(void)
{
    // open file 
    std::ifstream fileReader(sourceFile);
    if (!fileReader.is_open())
        return 1;
    
    while (fileReader)
    {
        std::string line;
        std::getline(fileReader, line);
        tokenize_line(line);
    }
    return 0;
}

void Lexer::tokenize_line(std::string line)
{
    const int ASCII_0 = 48;
    const int ASCII_9 = 57;
    this->currentLine = line;
    advance();
    while (this->position != -1)
    {
        // match single characters
        char character = (this->currentLine)[this->position];
        switch (character) // assign correct tokens
        {
            case ' ': // skip over whitespace
                advance();
                break;
            
            // simple single character tokens
            case '+':
            {
                advance();
                struct Token token("[ADDITION]", character);
                this->tokens.push_back(token);
                break;
            }
            case '-':
            {
                advance();
                struct Token token("[SUBTRACTION]", character);
                this->tokens.push_back(token);
                break;
            }
            case '*':
            {
                advance();
                struct Token token("[MULTIPLICATION]", character);
                this->tokens.push_back(token);
                break;
            }
            case '/':
            {
                advance();
                struct Token token("[DIVISION]", character);
                this->tokens.push_back(token);
                break;
            }
            case '(':
            {
                advance();
                struct Token token("[LBRACKET]", character);
                this->tokens.push_back(token);
                break;
            }
            case ')':
            {
                advance();
                struct Token token("[RBRACKET]", character);
                this->tokens.push_back(token);
                break;
            }
            case '[':
            {
                advance();
                struct Token token("[LSQUARE]", character);
                this->tokens.push_back(token);
                break;
            }
            case ']':
            {
                advance();
                struct Token token("[RSQUARE]", character);
                this->tokens.push_back(token);
                break;
            }
            case ';':
            {
                advance();
                struct Token token("[SEMICOLON]", character);
                this->tokens.push_back(token);
                break;
            }
            case ',':
            {
                advance();
                struct Token token("[COMMA]", character);
                this->tokens.push_back(token);
                break;
            }
            default:
            {
                // keywords, literals or identifiers
                if ((int)character >= ASCII_0 && (int)character <= ASCII_9) // integer literal
                {
                    std::string value = get_integer_literal();
                    struct Token token("[INTEGER_LITERAL]", value);
                    this->tokens.push_back(token);
                }
                else if (character == '\'')
                {
                    std::string value = get_string_literal();
                    struct Token token("[STRING_LITERAL]", value);
                    this->tokens.push_back(token);
                }
                else
                {
                    advance();
                    struct Token token("[UNKNOWN]", character);
                    this->tokens.push_back(token);
                }
                break;
            }
                
        };
    }
}

std::string Lexer::get_integer_literal()
{
    // form number by reading until non-integer character is reached
    const int ASCII_0 = 48;
    const int ASCII_9 = 57;
    std::string number;
    char currentCharacter = (this->currentLine)[this->position];
    while (((int)currentCharacter >= ASCII_0 && (int)currentCharacter <= ASCII_9) && this->position != -1)
    {
        number = number + currentCharacter;
        advance();
        currentCharacter = (this->currentLine)[this->position];
    }
    return number;
}

std::string Lexer::get_string_literal()
{
    // read up to ending character ) and create a string literal
    char currentCharacter = (this->currentLine)[this->position];
    std::string stringLiteral;
    stringLiteral = stringLiteral + currentCharacter; // add initial quotation mark
    advance(); 
    currentCharacter = (this->currentLine)[this->position];

    while (currentCharacter != '\'') // read until next character
    {
        stringLiteral = stringLiteral + currentCharacter;
        advance();
        currentCharacter = (this->currentLine)[this->position];
    }
    stringLiteral = stringLiteral + currentCharacter;
    advance();
    return stringLiteral;
}

void Lexer::advance()
{
    this->position = this->position + 1;
    if (this->position == (this->currentLine).length()) // check if reached end of line
        this->position = -1;
}