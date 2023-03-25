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
        if (line != "")
        {
            tokenize_line(line);
        }
    }
    // add final EOF token
    Token token(Tokens::END_OF_FILE, Keywords::END_OF_FILE);
    this->tokens.push_back(token);
    return 0;
}

void Lexer::tokenize_line(std::string line)
{
    const char WHITESPACE = ' ';
    this->currentLine = line;
    advance();
    
    while (this->position != -1) // read until end of line
    {
        char character = this->currentLine[this->position];
        if (this->SYMBOLS_TO_TOKENS.count(character)) // common single character tokens such as brackets, commas, colons etc
        {
            Token token = lookahead(character);
            this->tokens.push_back(token);
        }
        else if (is_letter(character)) // build a keyword or identifier from alphanumeric characters
        {
            Token token = get_keyword_or_identifier();
            this->tokens.push_back(token);
        }
        else if (is_integer(character))
        {
            Token token = get_numerical_literal();
            this->tokens.push_back(token);
        }
        else if (character == '"')
        {
            Token token = get_string_literal();
            this->tokens.push_back(token);
        }
        advance();
    }
}

Token Lexer::lookahead(char character)
{
    // "peeks" at the next two character to take the valid maximal munch of a potential token
    // when given two token choices < or or <= or <--, <-- will be chosen as its the longest valid token
    // return generated token
    if (character == Keywords::LESSER || character == Keywords::GREATER) // in psuedocode there are only two characters with double or triple operators
    {
        int offset = 2; // how many places to backtrack if a double or triple letter operator cannot be formed
        std::string largestOperator = {character};
        Token token(this->SYMBOLS_TO_TOKENS.at(character), character);

        for (int i = 1; i <= 2; i++) // look two steps ahead to match double or triple letter operators
        {
            if (this->position + i < this->currentLine.length())
            {
                advance();
                char nextCharacter = this->currentLine[this->position];
                largestOperator = largestOperator + nextCharacter;
                if (this->KEYWORDS_TO_TOKENS.count(largestOperator))
                {
                    token.type = this->KEYWORDS_TO_TOKENS.at(largestOperator);
                    token.value = largestOperator;
                    offset--;
                }
            }
        }
        this->position -= offset;
        return token;
    }
    Token token(this->SYMBOLS_TO_TOKENS.at(character), character);
    return token;
}

void Lexer::advance()
{
    this->position++;
    if (this->position >= this->currentLine.length() || this->position == -1) // check if reached end of line
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
        if (!is_alphanumeric(value[i]) || value[i] == WHITESPACE)
        {
            return false;
        }
    }
    return true;
}

Token Lexer::get_keyword_or_identifier(void)
{
    Token token;
    std::string buffer = {this->currentLine[this->position]};
    advance();
    while (is_alphanumeric(this->currentLine[this->position]) && this->position != -1)
    {
        buffer = buffer + this->currentLine[this->position];
        advance();
    }
    if (this->KEYWORDS_TO_TOKENS.count(buffer))
    {
        token.type = this->KEYWORDS_TO_TOKENS.at(buffer);
    }
    else if (buffer == Keywords::TRUE || buffer == Keywords::FALSE)
    {
        token.type = Tokens::BOOLEAN_LITERAL;
    }
    else if (is_valid_identifier(buffer))
    {
        token.type = Tokens::IDENTIFIER;
    }
    else
    {
        token.type = Tokens::UNKNOWN;
    }
    token.value = buffer;
    this->position--;
    return token;
}

Token Lexer::get_numerical_literal(void)
{
    // get either a float or integer literal depending on the presence of a decimal point
    std::string type = Tokens::INTEGER_LITERAL;
    std::string number;
    char character = this->currentLine[this->position];
    while ((is_integer(character) || character == '.') && this->position != -1)
    {
        if (character == '.')
        {
            type = Tokens::FLOAT_LITERAL;
        }
        number = number + character;
        advance();
        character = this->currentLine[this->position];
    }
    this->position--;
    Token token(type, number);
    return token;
}

Token Lexer::get_string_literal()
{
    // read up to ending character ) and create a string literal
    char character = this->currentLine[this->position];
    std::string stringLiteral;
    stringLiteral = stringLiteral + character; // add initial quotation mark
    advance(); 
    character = this->currentLine[this->position];
    while (character != '"') // read until next quotation mark
    {
        stringLiteral = stringLiteral + character;
        advance();
        character = this->currentLine[this->position];
    }
    stringLiteral = stringLiteral + character; // add closing quotation mark
    Token token(Tokens::STRING_LITERAL, stringLiteral);
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
    return false;
}

bool Lexer::is_letter(char character)
{
    const int ASCII_MIN_LOWER = 97;
    const int ASCII_MIN_UPPER = 65;
    const int ASCII_MAX_LOWER = 122;
    const int ASCII_MAX_UPPER = 90;
    int ascii = (int)character;

    if ((ascii >= ASCII_MIN_LOWER && ascii <= ASCII_MAX_LOWER) || (ascii >= ASCII_MIN_UPPER && ascii <= ASCII_MAX_UPPER))
    {
        return true;
    }
    return false;
}

bool Lexer::is_alphanumeric(char character)
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
    return false;
}