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
    std::string buffer;
    bool error = false;

    while (this->position != -1) // read until end of line
    {
        char character = (this->currentLine)[this->position];
        std::cout << character << std::endl;
        if (this->SYMBOLS_TO_TOKENS.count(character)) // match single characters
        {
            if (buffer != "")
            {
                process_buffer(buffer);
                buffer = "";
            }

            if (character == '=' || character == '>' || character == '<' || character == '!')
            {
                // lookahead to match double character operators
                std::string tmp(character, 1);
                advance();
                if (this->position != -1) // lookahead
                {
                    tmp = tmp + (this->currentLine)[this->position];
                    if (this->KEYWORDS_TO_TOKENS.count(tmp))
                    {
                        std::string type = this->KEYWORDS_TO_TOKENS.at(tmp);
                        struct Token token(type, tmp);
                        this->tokens.push_back(token);
                    }
                    else
                    {
                        // generate single character token
                        std::string type = this->SYMBOLS_TO_TOKENS.at(character);
                        struct Token token(type, character);
                        this->tokens.push_back(token);
                    }
                }
                else
                {
                    error = true;   
                }
            }
            else
            {
                // create single character token
                std::string type = this->SYMBOLS_TO_TOKENS.at(character);
                struct Token token(type, character);
                this->tokens.push_back(token);
            }
        }
        else if (is_integer(character)) // lex integer literals
        {
            if (buffer != "")
            {
                process_buffer(buffer);
                buffer = "";
            }

            std::string value = get_integer_literal();
            struct Token token("[INTEGER_LITERAL]", value);
            this->tokens.push_back(token);
        }
        else if (character == '\'') // lex string literals
        {
            if (buffer != "")
            {
                process_buffer(buffer);
                buffer = "";
            }
            std::string value = get_string_literal();
            struct Token token("[STRING_LITERAL]", value);
            this->tokens.push_back(token);
        }
        else
        {
            if (character != ' ')
            {
                std::cout << "Adding character " << character << std::endl;
                buffer = buffer + character;
            }
        }
        advance();
    }

    if (buffer != "")
    {
        process_buffer(buffer);
        buffer = "";
    }

    return;
}

void Lexer::process_buffer(std::string buffer)
{
    std::string type;
    for (int i = 0; i < buffer.length(); i++)
    {
        std::cout << "Char " << buffer[i] << std::endl;
    }
    // buffer processing for keywords
    if (this->KEYWORDS_TO_TOKENS.count(buffer))
        type = this->KEYWORDS_TO_TOKENS.at(buffer);
    else
        type = "[UNKNOWN]";

    struct Token token(type, buffer);
    this->tokens.push_back(token);
    return;
}

bool Lexer::is_integer(char character)
{
    const int ASCII_0 = 48;
    const int ASCII_9 = 57;
    if ((int)character >= ASCII_0 && (int)character <= ASCII_9)
        return true;
    else
        return false;
}

std::string Lexer::get_integer_literal()
{
    // form number by reading until non-integer character is reached
    const int ASCII_0 = 48;
    const int ASCII_9 = 57;
    std::string number;
    char character = (this->currentLine)[this->position];
    while (is_integer(character) && this->position != -1)
    {
        number = number + character;
        advance();
        character = (this->currentLine)[this->position];
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
    return stringLiteral;
}

void Lexer::advance()
{
    this->position = this->position + 1;
    if (this->position == (this->currentLine).length()) // check if reached end of line
        this->position = -1;
}