#include "compiler.h"
// branch test
Compiler::Compiler(std::string filename)
{
    targetFile = filename;
}

int Compiler::compile(void)
{
    int lexingProcess = begin_lexing();
    if (lexingProcess == 1) 
        return 1;
    return 0;
}

int Compiler::begin_lexing(void)
{
    std::ifstream fileReader(targetFile);
    if (!fileReader.is_open())
        return 1;
    
    while (fileReader)
    {
        std::string line;
        std::getline(fileReader, line);
        tokenize(line);
    }
    for (int i = 0; i < tokens.size(); i ++)
    {
        std::cout << (tokens[i]).name << std::endl;
    }
    return 0;
}

void Compiler::tokenize(std::string line)
{
    // tokenize a line from the target file and update the vector containing tokens

    // outline of process
    // repeat until end of line is reached
    // ingest single character at line
    // check if character + previous character is a valid keyword if so then add respective token
    // else check if single character is a valid keyword
    // if not check if current buffer is a valid keyword  add respective token if so then empty buffer
    // store current character in variable for previous characters
    // add current character to buffer

    std::string buffer = "";
    std::string previousCharacter = "";

    bool specialCharacterFound = false;
    int specialCharacterCount = 0;
    int lineLength = line.length();
    for (int i = 0; i < lineLength; i++)
    {   
        std::string character(1, line[i]);
        std::string doubleCharacter = previousCharacter + character; // form a possible double character (i.e == or >= or &&)    
        if (TOKEN_TABLE.count(doubleCharacter)) // if a valid double character keyword is found
        {
            std::cout << "Double character " + doubleCharacter << std::endl;
            struct Token token(TOKEN_TABLE.at(doubleCharacter), doubleCharacter);
            tokens.push_back(token);
        }
        else if (TOKEN_TABLE.count(character)) // if a valid single character keyword is found
        {
            std::cout << "Single character " + character << std::endl;
            std::string tokenValue = TOKEN_TABLE.at(character);
            struct Token token(tokenValue, character);
            tokens.push_back(token);
            specialCharacterFound = true;
            specialCharacterCount++;
        }
        else if (character == " " || (i + 1 > lineLength)) // process buffer if delimiter character is reached or if nearing the end of line
        {
            // buffer containing known keywords like "output" or "func" will be automatically found by a table lookup
            if (TOKEN_TABLE.count(buffer))
            {
                struct Token token(TOKEN_TABLE.at(buffer), buffer);
                tokens.push_back(token);
                buffer = "";
            }
        }

        previousCharacter = character;
        buffer = buffer + character;
    }
}

struct Token Compiler::determine_type(std::string buffer)
{
    // further processing to determine 
    const std::regex REGEX_STRINGS("^'.'$");
    const std::regex REGEX_INTEGERS("^[0-9]*$");
    const std::regex REGEX_IDENTIFIERS("^(?![0-9]*$)[a-zA-Z0-9]+$");
    
    struct Token token("[UNKNOWN]", buffer); // generate token
    if (TOKEN_TABLE.count(buffer)) // search if buffer is a valid keyword
        token.name = TOKEN_TABLE.at(buffer);
    else
    {
        // regex checks for data type checking
        if (buffer == "true" || buffer == "false")
            token.name = "[BOOL_LITERAL]";
        else if (regex_search(buffer, REGEX_STRINGS))
            token.name = "[STRING_LITERAL]";
        else if (regex_search(buffer, REGEX_INTEGERS))
            token.name = "[INT_LITERAL]";
        else if (regex_search(buffer, REGEX_IDENTIFIERS))
            token.name = "[IDENTIFIER]";
    }
    
    return token;
}