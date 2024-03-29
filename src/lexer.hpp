#ifndef Lexer_Hpp
#define Lexer_Hpp

// Lexer: For each line in the source file, Lexer will attempt to generate the correct tokens. Eventually the entire program will be represented as a vector of Tokens.

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>

#include "token.hpp"
#include "constants.hpp"
#include "exceptions.hpp"

class Lexer
{
	private:
		const std::unordered_map<std::string, std::string> KEYWORDS_TO_TOKENS = {
			{Keywords::FUNCTION, Tokens::FUNCTION}, {Keywords::WHILE, Tokens::WHILE}, {Keywords::OUTPUT, Tokens::OUTPUT}, {Keywords::INPUT, Tokens::INPUT},
			{Keywords::ASSIGNMENT, Tokens::ASSIGNMENT}, {Keywords::IF, Tokens::IF}, {Keywords::ELSE, Tokens::ELSE}, {Keywords::GREATER_EQUAL, Tokens::GREATER_EQUAL}, 
			{Keywords::LESSER_EQUAL, Tokens::LESSER_EQUAL}, {Keywords::NOT_EQUAL, Tokens::NOT_EQUAL}, {Keywords::FOR, Tokens::FOR}, {Keywords::AND, Tokens::AND}, 
			{Keywords::OR, Tokens::OR}, {Keywords::STRING, Tokens::STRING}, {Keywords::INTEGER, Tokens::INTEGER}, {Keywords::FLOAT, Tokens::FLOAT}, {Keywords::MOD, Tokens::MOD},
			{Keywords::DECLARE, Tokens::DECLARE}, {Keywords::NOT, Tokens::NOT}, {Keywords::BOOLEAN, Tokens::BOOLEAN}, {Keywords::ENDFUNCTION, Tokens::ENDFUNCTION}, 
			{Keywords::ENDWHILE, Tokens::ENDWHILE}, {Keywords::ENDIF, Tokens::ENDIF}, {Keywords::THEN, Tokens::THEN}, {Keywords::RETURNS, Tokens::RETURNS}, 
			{Keywords::RETURN, Tokens::RETURN}, {Keywords::NEXT, Tokens::NEXT}, {Keywords::TO, Tokens::TO}, {Keywords::STEP, Tokens::STEP}, 
			{Keywords::ADDITION, Tokens::ADDITION}, {Keywords::SUBTRACTION, Tokens::SUBTRACTION}, {Keywords::MULTIPLICATION, Tokens::MULTIPLICATION},
			{Keywords::EQUAL, Tokens::EQUAL}, {Keywords::DIVISION, Tokens::DIVISION}, {Keywords::GREATER, Tokens::GREATER}, {Keywords::LESSER, Tokens::LESSER},
			{Keywords::COLON, Tokens::COLON}, {Keywords::LBRACKET, Tokens::LBRACKET}, {Keywords::RBRACKET, Tokens::RBRACKET}, {Keywords::LSQUARE, Tokens::LSQUARE}, 
			{Keywords::RSQUARE, Tokens::RSQUARE}, {Keywords::COMMA, Tokens::COMMA}, {Keywords::ARRAY, Tokens::ARRAY}, {Keywords::OF, Tokens::OF}};
		std::string sourceFile;
		std::string currentLine;
		std::vector<std::string> sourceFileLines;
		int position;
		void tokenize_line(std::string line);
		void advance(void);
		Token lookahead(std::string character);
		bool is_letter(std::string character);
		bool is_integer(std::string character);
		bool is_alphanumeric(std::string character);
		bool is_valid_identifier(std::string value);
		Token get_keyword_or_identifier(void);
		Token get_string_literal(void);
		Token get_numerical_literal(void);

	public:
		Lexer(std::string filename);
		int generate_tokens(void);
		std::vector<Token> tokens;
		std::vector<std::string> get_source_file_lines(void);
};

#endif