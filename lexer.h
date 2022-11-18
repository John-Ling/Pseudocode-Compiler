#ifndef Lexer_H
#define Lexer_H

#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "token.h"

class Lexer
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
        std::string sourceFile;
        std::string currentLine;
        int position;
        
        int lex(void);
        void tokenize_line(std::string line);
        void advance();
        std::string get_integer_literal();
        std::string get_string_literal();
    public:
        Lexer(std::string filename);
        int generate_tokens(void);
        std::vector<struct Token> tokens;
};

#endif