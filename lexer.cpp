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
    {
        return 1;
    }
    
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
    const char WHITESPACE = ' ';
    this->currentLine = line;
    advance();
    std::string buffer;
    while (this->position != -1) // read until end of line
    {
        char character = (this->currentLine)[this->position];

        // events that will trigger a buffer check
        // specific single characters such as ( or , or <
        // the start of a potential integer or float literal
        // the start of a potential string literal
        // character is a whitespace which acts as a delimiter
        // end index is reached
        // a buffer check is to discern the type of keyword being used or the name of an identifier
        if (this->SYMBOLS_TO_TOKENS.count(character))
        {
            check_buffer(buffer);
            buffer = "";
            struct Token token = lookahead(character);
            this->tokens.push_back(token);
        }
        else if (is_integer(character))
        {
            check_buffer(buffer);
            buffer = "";
            struct Token token = get_numerical_literal();
            this->tokens.push_back(token);
        }
        else if (character == '"')
        {
            check_buffer(buffer);
            buffer = "";
            struct Token token = get_string_literal();
            this->tokens.push_back(token);
        }
        else if (character == WHITESPACE) // if encountered whitespace or end of line
        {
            check_buffer(buffer);
            buffer = "";
        }
        else if (this->position + 1 == (this->currentLine.length() - 1)) // when at the second to last character
        {
            // peek next character and determine if it is part of keyword/literal/identifier or is a symbol token
            char peekedCharacter = (this->currentLine)[this->position + 1];
            if (!this->SYMBOLS_TO_TOKENS.count(peekedCharacter))
            {
                buffer = buffer + character + peekedCharacter;
                advance();
            }
            check_buffer(buffer);
            buffer = "";
        }

        if (character != WHITESPACE && !this->SYMBOLS_TO_TOKENS.count(character)) // ignore symbols and whitespace
        {
            buffer = buffer + character;
        }
        advance();
    }
    return;
}

void Lexer::check_buffer(std::string buffer)
{
    // check if buffer is a valid keyword or symbol then add that respective token if so
    if (buffer == "")
    {
        return;
    }

    if (buffer.length() == 1)
    {
        char symbol = buffer[0];
        if (this->SYMBOLS_TO_TOKENS.count(symbol))
        {
            struct Token token = lookahead(symbol);
            this->tokens.push_back(token);
        }
    }
    else
    {
        if (this->KEYWORDS_TO_TOKENS.count(buffer))
        {
            struct Token token(this->KEYWORDS_TO_TOKENS.at(buffer), buffer);
            this->tokens.push_back(token);
        }
        else if (buffer == "TRUE" || buffer == "FALSE")
        {
            struct Token token("[BOOLEAN_LITERAL]", buffer);
            this->tokens.push_back(token);
        }
        else if (is_valid_identifier(buffer))
        {
            struct Token token("[IDENTIFIER]", buffer);
            this->tokens.push_back(token);
        }
    }
}

struct Token Lexer::lookahead(char character)
{
    // "peeks" at the next character to take the valid maximal munch of a potential token
    // when given two token choices = and ==, == will be chosen as its the longest valid token
    // return generated token
    if (character == '>' || character == '<')
    {
        // lookahead to match double character operators
        std::string largestOperator = {character};
        std::string doubleOperator = {character};
        struct Token token(this->SYMBOLS_TO_TOKENS.at(character), character);

        for (int i = 1; i <= 2; i++)
        {
            if (this->position + i < (this->currentLine).length())
            {
                advance();
                char nextCharacter = (this->currentLine)[this->position];
                largestOperator = largestOperator + nextCharacter;
                if (this->KEYWORDS_TO_TOKENS.count(largestOperator))
                {
                    token.type = this->KEYWORDS_TO_TOKENS.at(largestOperator);
                    token.value = largestOperator;
                }
            }
        }
        return token;
    }
    struct Token token(this->SYMBOLS_TO_TOKENS.at(character), character);
    return token;
}

void Lexer::advance()
{
    this->position = this->position + 1;
    if (this->position >= (this->currentLine).length()) // check if reached end of line
    {
        this->position = -1;
    }
    return;
}

bool Lexer::is_valid_identifier(std::string value)
{
    const char WHITESPACE = ' ';
    
    if (is_integer(value[0]))
    {
        return false;
    }
    for (int i = 0; i < value.length(); i++)
    {
        if (!is_valid_letter(value[i]) || value[i] == WHITESPACE)
        {
            return false;
        }
    }
    return true;
}

struct Token Lexer::get_numerical_literal()
{
    // get either a float or integer literal depending on the presence of a decimal point
    std::string type = "[INTEGER_LITERAL]";
    std::string number;
    char character = (this->currentLine)[this->position];
    while ((is_integer(character) || character == '.') && this->position != -1)
    {
        if (character == '.')
        {
            type = "[FLOAT_LITERAL]";
        }
        number = number + character;
        advance();
        character = (this->currentLine)[this->position];
    }
    this->position--;

    struct Token token(type, number);
    return token;
}

struct Token Lexer::get_string_literal()
{
    // read up to ending character ) and create a string literal
    char character = (this->currentLine)[this->position];
    std::string stringLiteral;
    stringLiteral = stringLiteral + character; // add initial quotation mark
    advance(); 
    character = (this->currentLine)[this->position];
    while (character != '"') // read until next quotation mark
    {
        stringLiteral = stringLiteral + character;
        advance();
        character = (this->currentLine)[this->position];
    }
    stringLiteral = stringLiteral + character; // add closing quotation mark
    struct Token token("[STRING_LITERAL]", stringLiteral);
    return token;
}

bool Lexer::is_integer(char character)
{
    const int ASCII_0 = 48;
    const int ASCII_9 = 57;
    if ((int)character >= ASCII_0 && (int)character <= ASCII_9)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Lexer::is_valid_letter(char character)
{
    // checks if letter is character from a-z or A-Z or 0-9
    const int ASCII_MIN_LOWER = 97;
    const int ASCII_MIN_UPPER = 65;
    const int ASCII_MAX_LOWER = 122;
    const int ASCII_MAX_UPPER = 90;
    const int ASCII_0 = 48;
    const int ASCII_9 = 57;

    int ascii = (int)character;
    if ((ascii >= ASCII_MIN_LOWER && ascii <= ASCII_MAX_LOWER) || (ascii >= ASCII_MIN_UPPER && ascii <= ASCII_MAX_UPPER) || (ascii >= ASCII_0 && ascii <= ASCII_9))
    {
        return true;
    }
    else
    {
        return false;
    }
}