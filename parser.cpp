#include "parser.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->pointer = 0;
}

int Parser::parse_tokens(void)
{
    while (this->pointer < this->tokens.size())
	{
		std::cout << "Looking at token " << this->tokens[this->pointer].type << std::endl;
		int result = statement(); // parse token pointed to by this->pointer
		if (result == 1)
		{
			std::cout << "Failed" << std::endl;
			return 1;
		}
		else
		{
			this->pointer++;
		}
	}
	return 0;
}

int Parser::match(std::string tokenType)
{
	if (this->pointer >= this->tokens.size())
	{
		return 1;
	}
	else if (this->tokens[this->pointer].type == tokenType)
	{
		return 0;
	}
}

int Parser::statement(void)
{
	int result = 1;
	std::cout << this->tokens[this->pointer].type << std::endl;
    if (match("[OUTPUT]") == 0)
    {
		// generate an output node
		std::cout << "Going to function OUTPUT" << std::endl;
		this->pointer++;
		result = output();
	}
	else if (match("[FUNCTION]") == 0)
	{
		// generate a function declaration node
		std::cout << "Function" << std::endl;
		this->pointer++;
		result = function();
	}
	else if (match("[DECLARE]"))
	{
		// generate a variable declaration node
		std::cout << "Declare" << std::endl;
		this->pointer++;
		result = variable_declaration();

	}
	else if (match("[INPUT]"))
	{
		// generate an input node
		std::cout << "Input" << std::endl;
		this->pointer++;
		result = input();
	}
	else
	{
		// attempt to generate a standalone expression node
		result = expression();
	}
	return result;
}

int Parser::output(void)
{
	// <output> ::= [OUTPUT] <expression>
	if (expression() == 1)
	{
		return 1;
	}
    return 0;
}

int Parser::input(void)
{
	// <input> ::= [INPUT] [IDENTIFIER] <statement>
	if (this->tokens[this->pointer].type != "[IDENTIFIER]")
	{
		return 1;
	}
	return 0;
}

int Parser::function(void)
{
	// <function> ::= [FUNCTION] [IDENTIFIER] ( <function-parameter> [RETURNS] <primitive-type> <statement> [ENDFUNCTION]
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (match("[IDENTIFIER]") == 1)
	{
		return 1;
	}
	this->pointer++;
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (match("[LBRACKET]") == 1)
	{
		return 1;
	}
	this->pointer++;
	if (match("[RBRACKET]") == 0) // function without parameters
	{
		std::cout << "Checking potential empty function" << std::endl;
		this->pointer++;
		if (match("[RETURNS]") == 1)
		{
			return 1;
		}
		this->pointer++;
		if (primitive_type() == 1)
		{
			return 1;
		}
		std::cout << "Valid empty function" << std::endl;
		return 0; 
	}
	else
	{
		std::cout << "Checking function parameters" << std::endl;
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (this->tokens[this->pointer].type != "[IDENTIFIER]")
		{
			return 1;
		}
		this->pointer++;
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (this->tokens[this->pointer].type != "[COLON]")
		{
			return 1;
		}
		this->pointer++;
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (primitive_type() == 1)
		{
			return 1;
		}
		this->pointer++;
		if (function_parameter() == 1)
		{
			return 1;
		}
	}
	this->pointer++;
	if (this->tokens[this->pointer].type != "[RETURNS]")
	{
		return 1;
	}
	this->pointer++;
	if (primitive_type() == 1)
	{
		return 1;
	}
	std::cout << "Valid function" << std::endl;
	return 0;
}

int Parser::function_parameter(void)
{
	// <function-parameter> ::= ) | , [IDENTIFIER] : <primitive-literal> <function-parameter>
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (this->tokens[this->pointer].type == "[RBRACKET]")
	{
		std::cout << "reached end" << std::endl;
		return 0;
	}
	else if (this->tokens[this->pointer].type == "[COMMA]")
	{
		this->pointer++;
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (this->tokens[this->pointer].type != "[IDENTIFIER]")
		{
			return 1;
		}
		this->pointer++;
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (this->tokens[this->pointer].type != "[COLON]")
		{
			return 1;
		}
		this->pointer++;
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (primitive_type() == 1)
		{
			std::cout << "Failed literal test in function_parameter()" << std::endl;
			return 1;
		}
		this->pointer++;
		return function_parameter(); // recursively check if there are any more valid arguments and return the correct status integer
	}
	std::cout << "Reached here" << std::endl;
	return 1;
}

int Parser::variable_declaration(void)
{
	// <variable-declaration> ::= [DECLARE] [IDENTIFIER] : <primitive-type>
	if (this->tokens[this->pointer].type != "[IDENTIFIER]")
	{
		return 1;
	}
	this->pointer++;
	if (this->tokens[this->pointer].type != "[COLON]")
	{
		return 1;
	}
	this->pointer++;
	if (primitive_type() == 1)
	{
		return 1;
	}
	std::cout << "valid declaration" << std::endl;
	return 0;
}

int Parser::expression(void)
{
	// <expression> ::= 
	std::cout << this->tokens[this->pointer].type << std::endl;
	const int savedPointer = this->pointer; // backtrack to this index when 

	if (match("[NOT]") == 0) // unary expression
	{
		this->pointer++;
		if (expression() == 1)
		{
			return 1;
		}
		return 0;
	}
	if (match("[IDENTIFIER]") == 0)
	{
		this->pointer++;
		// attempt to generate function call node
	}
	if (match("[LBRACKET]") == 0) // bracketed expression
	{
		this->pointer++;
		if (expression() == 1)
		{
			return 1;
		}
		this->pointer++;
		if (match("[RBRACKET]") == 0)
		{
			std::cout << "Valid bracketed expression" << std::endl;
			return 0;
		}
		return 1;
	}
	if (match("[ADDITION]")  == 0 || match("[SUBTRACTION]") == 0)
	{
		// generate addition or subtraction node
		if (this->tokens[this->pointer].type == "[ADDITION]")
		{
			// generate an addition node
			this->pointer++;
			;	
		}
		else if (this->tokens[this->pointer].type == "[SUBTRACTION]")
		{
			this->pointer++;
			// generate subtraction node
			;
		}
	}
	if (operator_(this->pointer + 1) == 0) // binary expression
	{
		// <binary-expression> :: = <expression> <operator> <expression>
		// evaluate first expression

		// evaluate second expression
		this->pointer++;
		if (expression() == 1)
		{
			return;
		}
		std::cout << "Valid binary expression" << std::endl;
		return 0;
	}

	if (primitive_literal() == 0)
	{
		std::cout << "Primitive literal" << std::endl;
		return 0;
	}
	return 1;
}

int Parser::function_call(void)
{
	if (match("[LBRACKET]") == 1)
	{

	}
}

int Parser::primitive_literal(void)
{

	if (this->tokens[this->pointer].type == "[INTEGER_LITERAL]" ||
		this->tokens[this->pointer].type == "[FLOAT_LITERAL]"   ||
		this->tokens[this->pointer].type == "[STRING_LITERAL]"  || 
		this->tokens[this->pointer].type == "[BOOLEAN_LITERAL]")
	{
		return 0;        
	}
	return 1;
}

int Parser::primitive_type(void)
{
	if (this->tokens[this->pointer].type == "[INTEGER]" ||
		this->tokens[this->pointer].type == "[STRING]"  || 
		this->tokens[this->pointer].type == "[BOOLEAN]" || 
		this->tokens[this->pointer].type == "[FLOAT]")
	{
		return 0;
	}
	return 1;
}

int Parser::operator_(int pointer)
{
    if (this->tokens[pointer].type == "[ADDITION]"       || 
        this->tokens[pointer].type == "[SUBTRACTION]"    ||
        this->tokens[pointer].type == "[MULTIPLICATION]" ||
        this->tokens[pointer].type == "[DIVISION]"       || 
        this->tokens[pointer].type == "[MODULUS]")
    {
		std::cout << "This is an operator" << std::endl;
        return 0;
    }
    return 1;
}

int Parser::binary_expression(int pointer)
{
	// binary expression ::= <expression> <operator> <expression>
	std::cout << "Binary expression" << std::endl; 
	if (operator_(this->pointer + 1) == 1)
	{
		return 1;
	}
	
    return 1;
}

int Parser::unary_expression(int pointer)
{
    if ((this->tokens[pointer].type == "[NOT]" || this->tokens[pointer].type == "[SUBTRACTION]") && expression() == 0)
    {
        return 0;
    }
    return 1;
}
