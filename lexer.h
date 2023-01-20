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
            {"AND", "[AND]"}, {"OR", "[OR]"}, {"STRING", "[STRING]"}, {"INTEGER", "[INTEGER]"}, {"REAL", "[FLOAT]"}, {"MOD", "[MODULUS]"},
            {"DECLARE", "[DECLARE]"}, {"NOT", "[NOT]"}, {"BOOLEAN", "[BOOLEAN]"}, {"ENDFUNCTION", "[ENDFUNCTION]"}, {"ENDWHILE", "[ENDWHILE]"},
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
        void tokenize_line(std::string line);
        void advance();
        void check_buffer(std::string buffer);
        Token lookahead(char character);
        bool is_integer(char character);
        bool is_valid_letter(char character);
        bool is_valid_identifier(std::string value);
        Token get_string_literal();
        Token get_numerical_literal();
    public:
        Lexer(std::string filename);
        int generate_tokens(void);
        std::vector<Token> tokens;
};

#endif