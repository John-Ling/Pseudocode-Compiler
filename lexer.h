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
        const std::unordered_map<std::string, std::string> KEYWORDS_TO_TOKENS = {
            {"while", "[WHILE]"}, {"output", "[OUTPUT]"}, {"input", "[INPUT]"}, {"if", "[IF]"}, {"else", "[ELSE]"},
            {">=", "[GREATER_EQUAL]"}, {"<=", "[LESSER_EQUAL]"}, {"==", "[EQUALS]"}, {"!=", "[NOT_EQUALS]"},
            {"&&", "[AND]"}, {"||", "[OR]"}, {"string", "[STRING]"}, {"integer", "[INTEGER]"}, 
            {"bool", "[BOOLEAN]"}, {"end", "[END]"}
        }; 
        const std::unordered_map<char, std::string> SYMBOLS_TO_TOKENS {
            {'+', "[ADDITION]"}, {'-', "[SUBTRACTION]"}, {'*', "[MULTIPLICATION]"},
            {'/', "[DIVISION]"}, {'=', "[ASSIGNMENT]"}, {'>', "[GREATER]"}, {'<', "[LESSER]"},
            {'!', "[NOT]"}, {'(', "[LBRACKET]"}, {')', "[RBRACKET]"},
            {'[', "[LSQUARE]"}, {']', "[RSQUARE]"}, {',', "[COMMA]"}, {';', "[SEMICOLON]"}
        };
        std::string sourceFile;
        std::string currentLine;
        int position;
        
        int lex(void);
        void tokenize_line(std::string line);
        void advance();
        void lookahead();
        void process_buffer(std::string buffer);
        bool is_integer(char character);
        std::string get_integer_literal();
        std::string get_string_literal();
    public:
        Lexer(std::string filename);
        int generate_tokens(void);
        std::vector<struct Token> tokens;
};

#endif