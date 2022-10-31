#include "compiler.h"

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
    int lineLength = line.length();
    for (int i = 0; i < lineLength; i++)
    {   
        std::string character(1, line[i]);
        std::string doubleCharacter = previousCharacter + character; // form a possible double character (i.e == or >= or &&)    
        std::cout << "Current character "  + character << std::endl;
        std::cout << doubleCharacter << std::endl;
        if (TOKEN_TABLE.count(doubleCharacter))
        {
            std::cout << "Found double character" << std::endl;
            struct Token token(TOKEN_TABLE.at(doubleCharacter), doubleCharacter);
            tokens.push_back(token);
            specialCharacterFound = true;
        }
        else if (TOKEN_TABLE.count(character))
        {
            std::cout << "Found single character" << std::endl;
            struct Token token(TOKEN_TABLE.at(character), character);
            std::cout << token.name << std::endl;
            std::cout << token.value << std::endl;
            tokens.push_back(token);
            specialCharacterFound = true;
        }
        else if (character == " " || specialCharacterFound || (i + 1 > lineLength)) // process buffer if delimiter character or special character is reached or process if nearing the end of line
        {
            // process contents of buffer
            struct Token token("", "");
            std::cout << buffer << std::endl;

            if (TOKEN_TABLE.count(buffer))
            {
                token.name = TOKEN_TABLE.at(buffer);
                token.value = buffer;
            }
            else
                token = determine_type(buffer);
            tokens.push_back(token);
            buffer = "";
        }

        specialCharacterFound = false;
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
            token.name = "[BOOLEAN]";
        else if (regex_search(buffer, REGEX_STRINGS))
            token.name = "[STRING]";
        else if (regex_search(buffer, REGEX_INTEGERS))
            token.name = "[INTEGER]";
        else if (regex_search(buffer, REGEX_IDENTIFIERS))
            token.name = "[IDENTIFIER]";
    }
    
    return token;
}