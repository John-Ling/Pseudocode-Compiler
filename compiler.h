#ifndef Compiler_H
#define Compiler_H
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include "token.h"

class Compiler
{    
    private:
        const std::string SYMBOLS = "()*/+-,><[]="; // acceptable symbols in the language
        const std::unordered_map<std::string, std::string> KEYWORDS_TO_TOKENS = {
            {"func", "[FUNC]"}, {"while", "[WHILE]"}, {"return", "[RETURN]"},
            {"out", "[OUTPUT]"}, {"in", "[INPUT]"}, {"if", "[IF]"}, {"else", "[ELSE]"},
            {"+", "[ADDITION]"}, {"-", "[SUBTRACTION]"}, {"*", "[MULTIPLICATION]"},
            {"/", "[DIVISION]"}, {"=", "[ASSIGNMENT]"}, {">", "[GREATER]"}, {"<", "[LESSER]"},
            {">=", "[GREATER_EQUAL]"}, {"<=", "[LESSER_EQUAL]"}, {"==", "[EQUALS]"}, {"!=", "[NOT_EQUALS]"},
            {"&&", "[AND]"}, {"||", "[OR]"}, {"!", "[NOT]"}, {"(", "[LBRACKET]"}, {")", "[RBRACKET]"},
            {"[", "[LSQUARE]"}, {"]", "[RSQUARE]"}, {",", "[COMMA]"}, {";", "[SEMICOLON]"}, {"end", "[END]"}
        };

        std::vector<std::string> tokens;
        std::string targetFile;
        int begin_lexing();
        void tokenize(std::string line);
        void process_buffer(std::string buffer);
    public:
        Compiler(std::string filename);
        int compile();
};

#endif