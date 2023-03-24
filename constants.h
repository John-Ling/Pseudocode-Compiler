#ifndef Constants_H
#define Constants_H

#include <string>

// TOKEN KEYWORDS
namespace Tokens
{
	const std::string FUNCTION = "[FUNCTION]";
	const std::string WHILE = "[WHILE]";
	const std::string OUTPUT = "[OUTPUT]";
	const std::string INPUT = "[INPUT]";
	const std::string ASSIGNMENT = "[ASSIGNMENT]";
	const std::string IF = "[IF]";
	const std::string ELSE = "[ELSE]";
	const std::string GREATER_EQUAL = "[GREATER_EQUAL]";
	const std::string LESSER_EQUAL = "[LESSER_EQUAL]";
	const std::string NOT_EQUAL = "[NOT_EQUAL]";
	const std::string FOR = "[FOR]";
	const std::string AND = "[AND]";
	const std::string OR = "[OR]";
	const std::string STRING = "[STRING]";
	const std::string INTEGER = "[INTEGER]";
	const std::string FLOAT = "[FLOAT]";
	const std::string MOD = "[MODULUS]";
	const std::string DECLARE = "[DECLARE]";
	const std::string NOT = "[NOT]";
	const std::string BOOLEAN = "[BOOLEAN]";
	const std::string ENDFUNCTION = "[ENDFUNCTION]";
	const std::string ENDWHILE = "[ENDWHILE]";
	const std::string ENDIF = "[ENDIF]";
	const std::string THEN = "[THEN]";
	const std::string RETURNS = "[RETURNS]";
	const std::string RETURN = "[RETURN]";
	const std::string NEXT = "[NEXT]";
	const std::string TO = "[TO]";
	const std::string STEP = "[STEP]";

	// TOKEN CHARACTERS
	const std::string ADDITION = "[ADDITION]";
	const std::string SUBTRACTION = "[SUBTRACTION]";
	const std::string MULTIPLICATION = "[MULTIPLICATION]";
	const std::string DIVISION = "[DIVISION]";
	const std::string EQUAL = "[EQUAL]";
	const std::string GREATER = "[GREATER]";
	const std::string LESSER = "[LESSER]";
	const std::string COLON = "[COLON]";
	const std::string LBRACKET = "[LBRACKET]";
	const std::string RBRACKET = "[RBRACKET]";
	const std::string LSQUARE = "[LSQUARE]";
	const std::string RSQUARE = "[RSQUARE]";
	const std::string COMMA = "[COMMA]";

	// LITERALS AND IDENTIFIER
	const std::string BOOLEAN_LITERAL = "[BOOLEAN_LITERAL]";
	const std::string STRING_LITERAL = "[STRING_LITERAL]";
	const std::string INTEGER_LITERAL = "[INTEGER_LITERAL]";
	const std::string FLOAT_LITERAL = "[FLOAT_LITERAL]";
	const std::string IDENTIFIER = "[IDENTIFIER]";
	const std::string UNKNOWN = "[UNKNOWN]";

	// Misc
	const std::string END_OF_FILE = "[EOF]";
}

namespace Keywords
{
	// PSEUDOCODE KEYWORDS
	const std::string FUNCTION = "FUNCTION";
	const std::string WHILE = "WHILE";
	const std::string OUTPUT = "OUTPUT";
	const std::string INPUT = "INPUT";
	const std::string ASSIGNMENT = "<--";
	const std::string IF = "IF";
	const std::string ELSE = "ELSE";
	const std::string GREATER_EQUAL = ">=";
	const std::string LESSER_EQUAL = "<=";
	const std::string NOT_EQUAL = "<>";
	const std::string FOR = "FOR";
	const std::string AND = "AND";
	const std::string OR = "OR";
	const std::string STRING = "STRING";
	const std::string INTEGER = "INTEGER";
	const std::string FLOAT = "REAL";
	const std::string MOD = "MOD";
	const std::string DECLARE = "DECLARE";
	const std::string NOT = "NOT";
	const std::string BOOLEAN = "BOOLEAN";
	const std::string ENDFUNCTION = "ENDFUNCTION";
	const std::string ENDWHILE = "ENDWHILE";
	const std::string ENDIF = "ENDIF";
	const std::string THEN = "THEN";
	const std::string RETURNS = "RETURNS";
	const std::string RETURN = "RETURN";
	const std::string NEXT = "NEXT";
	const std::string TO = "TO";
	const std::string STEP = "STEP";

	// SINGLE CHARACTER KEYWORDS
	const char ADDITION = '+';
	const char SUBTRACTION = '-';
	const char MULTIPLICATION = '*';
	const char DIVISION = '/';
	const char EQUAL = '=';
	const char GREATER = '>';
	const char LESSER = '<';
	const char COLON = ':';
	const char LBRACKET = '(';
	const char RBRACKET = ')';
	const char LSQUARE = '[';
	const char RSQUARE = ']';
	const char COMMA = ',';

	// MISC
	const std::string TRUE = "TRUE";
	const std::string FALSE = "FALSE";
	const std::string END_OF_FILE = "EOF";
}

#endif