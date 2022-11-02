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
    bool openedQuote = false;
    std::string buffer = "";

    for (int i = 0; i < line.length(); i++)
    {
        std::string character(1, line[i]);
        if (character == "'" && openedQuote)
        {
            // generate string token
            struct Token token("[STRING_LITERAL]", buffer);
            tokens.push_back(token);
            buffer = "";
            openedQuote = false;
        }
        else if (character == "'")
            openedQuote = true;

        if (!openedQuote)
        {
            if (character == " " || TOKEN_TABLE.count(character))
            {
                struct Token token("", "");
                struct Token characterToken("", "");
                bool characterFound = false;
                token = determine_type(buffer);
                if (TOKEN_TABLE.count(character))
                {
                    // lookahead
                    if (!(i + 1 == line.length()))
                    {
                        std::string doubleCharacter = character + line[i];
                        if (TOKEN_TABLE.count(doubleCharacter))
                        {
                            characterToken.name = TOKEN_TABLE.at(doubleCharacter);
                            characterToken.value = doubleCharacter;
                        }
                    }
                    else
                    {
                        characterToken.name = TOKEN_TABLE.at(character);
                        characterToken.value = character;
                    }
                    characterFound = true;
                }

                tokens.push_back(token);
                if (characterFound)
                    tokens.push_back(characterToken);
                buffer = "";
            }
        }
        buffer = buffer + character;
    }
}

struct Token Compiler::determine_type(std::string buffer)
{
    // further processing to determine 
    const std::regex REGEX_STRINGS("^'.*\n*'$");
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