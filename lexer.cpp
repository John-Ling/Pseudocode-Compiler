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

    for (int i = 0; i < this->tokens.size(); i++)
    {
        std::cout << this->tokens[i].type << " " << this->tokens[i].value << std::endl;
    }
    return 0;
}

void Lexer::tokenize_line(std::string line)
{
	// add characters to a buffer and examine them once a delimiter is reached
	// reaching a quotation mark " or integer will trigger functions that take over regular operation and form the integer 
	// or string rather than adding to buffer
    const char WHITESPACE = ' ';
    this->currentLine = line;
    advance();
    int maxIndex = line.length() - 1;
    std::string buffer;
    while (this->position != -1) // read until end of line
    {
        char character = this->currentLine[this->position];

        // example delimiters
        // specific single characters such as ( or , or <
        // the start of a potential integer or float literal
        // the start of a potential string literal
        // character is a whitespace which acts as a delimiter
        // a buffer check is to discern the type of keyword being used or the name of an identifier

        if (this->SYMBOLS_TO_TOKENS.count(character))
        {
            check_buffer(buffer);
            buffer = "";
            Token token = lookahead(character);
            this->tokens.push_back(token);
        }
        else if (is_integer(character))
        {
            check_buffer(buffer);
            buffer = "";
            Token token = get_numerical_literal();
            this->tokens.push_back(token);
        }
        else if (character == '"')
        {
            check_buffer(buffer);
            buffer = "";
            Token token = get_string_literal();
            this->tokens.push_back(token);
        }
        else if (character == WHITESPACE) // if encountered whitespace or end of line
        {
            check_buffer(buffer);
            buffer = "";
        }

        if (character != WHITESPACE && !this->SYMBOLS_TO_TOKENS.count(character)) // ignore symbols and whitespace
        {
            buffer = buffer + character;
        }
        advance();
    }
    check_buffer(buffer);
    return;
}

void Lexer::check_buffer(std::string buffer)
{
    const std::string BOOLEAN_LITERAL = "[BOOLEAN_LITERAL]";
    const std::string IDENTIFIER = "[IDENTIFIER]";

    // check if buffer is a valid keyword or symbol then add that respective token if so
    if (buffer == "")
    {
        return;
    }

    if (this->SYMBOLS_TO_TOKENS.count(buffer[0]))
    {
        Token token = lookahead(buffer[0]);
        this->tokens.push_back(token);
    }
    else if (this->KEYWORDS_TO_TOKENS.count(buffer))
    {
        Token token(this->KEYWORDS_TO_TOKENS.at(buffer), buffer);
        this->tokens.push_back(token);
    }
    else if (buffer == "TRUE" || buffer == "FALSE")
    {
        Token token(BOOLEAN_LITERAL, buffer);
        this->tokens.push_back(token);
    }
    else if (is_valid_identifier(buffer))
    {
        Token token(IDENTIFIER, buffer);
        this->tokens.push_back(token);
    }
}

Token Lexer::lookahead(char character)
{
    // "peeks" at the next two character to take the valid maximal munch of a potential token
    // when given two token choices < or or <= or <--, <-- will be chosen as its the longest valid token
    // return generated token
    if (character == '>' || character == '<') // in psuedocode there are only two characters with double or triple operators
    {
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
                }
            }
        }
        return token;
    }
    Token token(this->SYMBOLS_TO_TOKENS.at(character), character);
    return token;
}

void Lexer::advance()
{
    this->position++;
    if (this->position >= this->currentLine.length()) // check if reached end of line
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

Token Lexer::get_numerical_literal()
{
    // get either a float or integer literal depending on the presence of a decimal point
    const std::string INTEGER_LITERAL = "[INTEGER_LITERAL]";
    const std::string FLOAT_LITERAL = "[FLOAT_LITERAL]";

    std::string type = INTEGER_LITERAL;
    std::string number;
    char character = this->currentLine[this->position];
    while ((is_integer(character) || character == '.') && this->position != -1)
    {
        if (character == '.')
        {
            type = FLOAT_LITERAL;
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
    const std::string STRING_LITERAL = "[STRING_LITERAL]";
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
    Token token(STRING_LITERAL, stringLiteral);
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