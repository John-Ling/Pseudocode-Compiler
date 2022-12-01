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
            {"func", "[FUNC]"}, {"while", "[WHILE]"}, {"output", "[OUTPUT]"}, {"input", "[INPUT]"}, 
            {"if", "[IF]"}, {"else", "[ELSE]"}, {">=", "[GREATER_EQUAL]"}, {"<=", "[LESSER_EQUAL]"}, 
            {"==", "[EQUAL]"}, {"!=", "[NOT_EQUAL]"},
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
        struct Token lookahead(char character);
        int process_buffer(std::string buffer);
        void add_token(std::string value);
        void add_token(char character);
        bool is_integer(char character);
        bool is_valid_letter(char character);
        bool is_valid_identifier(std::string value);
        struct Token get_integer_literal();
        struct Token get_string_literal();
    public:
        Lexer(std::string filename);
        int generate_tokens(void);
        std::vector<struct Token> tokens;
};

#endif