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
            {"FUNCTION", "[FUNCTION]"}, {"WHILE", "[WHILE]"}, {"OUTPUT", "[OUTPUT]"}, {"INPUT", "[INPUT]"}, {"<--", "[ASSIGNMENT]"},
            {"IF", "[IF]"}, {"ELSE", "[ELSE]"}, {">=", "[GREATER_EQUAL]"}, {"<=", "[LESSER_EQUAL]"}, {"<>", "[NOT_EQUAL]"},
            {"AND", "[AND]"}, {"OR", "[OR]"}, {"STRING", "[STRING]"}, {"INTEGER", "[INTEGER]"}, {"REAL", "[FLOAT]"},
            {"DECLARE", "[DECLARE]"}, {"NOT", "[NOT]"}, {"bool", "[BOOLEAN]"}, {"ENDFUNCTION", "[ENDFUNCTION]"}, {"ENDWHILE", "[ENDWHILE]"},
            {"ENDIF", "[ENDIF]"}, {"THEN", "[THEN]"}, {"RETURNS", "[RETURNS]"}, {"RETURN", "[RETURN]"}
        }; 
        const std::unordered_map<char, std::string> SYMBOLS_TO_TOKENS {
            {'+', "[ADDITION]"}, {'-', "[SUBTRACTION]"}, {'*', "[MULTIPLICATION]"}, {'=', "[EQUAL]"},
            {'/', "[DIVISION]"}, {'>', "[GREATER]"}, {'<', "[LESSER]"}, {':', "[COLON]"},
            {'(', "[LBRACKET]"}, {')', "[RBRACKET]"}, {'[', "[LSQUARE]"}, {']', "[RSQUARE]"}, {',', "[COMMA]"}
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
        struct Token get_string_literal();
        struct Token get_numerical_literal();
    public:
        Lexer(std::string filename);
        int generate_tokens(void);
        std::vector<struct Token> tokens;
};

#endif