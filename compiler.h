#ifndef Compiler_H
#define Compiler_H
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include "lexer.h"

class Compiler
{    
    private:
        const std::unordered_map<std::string, std::string> TOKEN_TABLE = {
            {"while", "[WHILE]"}, {"output", "[OUTPUT]"}, {"input", "[INPUT]"}, {"if", "[IF]"}, {"else", "[ELSE]"},
            {"+", "[ADDITION]"}, {"-", "[SUBTRACTION]"}, {"*", "[MULTIPLICATION]"},
            {"/", "[DIVISION]"}, {"=", "[ASSIGNMENT]"}, {">", "[GREATER]"}, {"<", "[LESSER]"},
            {">=", "[GREATER_EQUAL]"}, {"<=", "[LESSER_EQUAL]"}, {"==", "[EQUALS]"}, {"!=", "[NOT_EQUALS]"},
            {"&&", "[AND]"}, {"||", "[OR]"}, {"!", "[NOT]"}, {"(", "[LBRACKET]"}, {")", "[RBRACKET]"},
            {"[", "[LSQUARE]"}, {"]", "[RSQUARE]"}, {",", "[COMMA]"}, {";", "[SEMICOLON]"}, 
            {"string", "[STRING]"}, {"integer", "[INTEGER]"}, {"bool", "[BOOLEAN]"}, {"end", "[END]"}
        };

        std::string targetFile;
        int begin_lexing(void);
        void tokenize(std::string line);
    public:
        Compiler(std::string filename);
        int compile(void);
};

#endif