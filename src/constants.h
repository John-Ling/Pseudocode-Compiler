#ifndef Constants_H
#define Constants_H

#include <string>

namespace Keywords // modify to change the keyword for a specific action i.e change "RETURN" to "wumbo" to make "wumbo" the return keyword.
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
	const std::string ADDITION = "+";
	const std::string SUBTRACTION = "-";
	const std::string MULTIPLICATION = "*";
	const std::string DIVISION = "/";
	const std::string EQUAL = "=";
	const std::string GREATER = ">";
	const std::string LESSER = "<";
	const std::string COLON = ":";
	const std::string LBRACKET = "(";
	const std::string RBRACKET = ")";
	const std::string LSQUARE = "[";
	const std::string RSQUARE = "]";
	const std::string COMMA = ",";

	// MISC
	const std::string TRUE = "TRUE";
	const std::string FALSE = "FALSE";
	const std::string END_OF_FILE = "EOF";
}


// TOKEN KEYWORDS
namespace Tokens // Internal token names for compiler to use. I wouldn't touch these.
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
	const std::string END_OF_LINE = "[EOL]";
}

namespace AST_Node_Names // Internal node names for abstract syntax tree. I wouldn't touch these.
{
	const std::string FUNCTION = "function";
	const std::string WHILE = "while";
	const std::string IF = "if";
	const std::string ELSE = "else";
	const std::string FOR = "for";
	const std::string RETURN = "return";
	const std::string LITERAL = "literal";
	const std::string IDENTIFIER = "identifier";
	const std::string PRIMITIVE = "primitive";
	const std::string DECLARATION = "variable_declaration";
	const std::string ASSIGNMENT = "variable_assignment";
	const std::string OUTPUT = "output";
	const std::string INPUT = "input";
	const std::string BINARY_EXPRESSION = "binary_expression";
	const std::string UNARY_EXPRESSION = "unary_expression";
	const std::string FUNCTION_ARGUMENTS = "function_arguments";
	const std::string CALL_ARGUMENTS = "call_arguments";
}

namespace Exception_Types // Internal exception names for compile. I wouldn't touch these
{
	const std::string GENERIC = "generic";
	const std::string UNEXPECTED_TOKEN = "unexpected_token";
	const std::string MISSED_TERMINATOR = "missed_arnold";
	const std::string LEXICAL = "lexical";
	const std::string INCORRECT_TYPE = "bad-type";
	const std::string END = "end";

}

#endif