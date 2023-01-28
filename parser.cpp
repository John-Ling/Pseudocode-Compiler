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

int Parser::statement(void)
{
	int result = 1;
	std::cout << this->tokens[this->pointer].type << std::endl;
    if (this->tokens[this->pointer].type == "[OUTPUT]")
    {
		std::cout << "Going to function OUTPUT" << std::endl;
		result = output();
	}
	else if (this->tokens[this->pointer].type == "[FUNCTION]")
	{
		std::cout << "Function" << std::endl;
		this->pointer++;
		result = function();
	}
	else if (this->tokens[this->pointer].type == "[DECLARE]")
	{
		std::cout << "Declare" << std::endl;
		this->pointer++;
		result = variable_declaration();

	}
	else if (this->tokens[this->pointer].type == "[INPUT]")
	{
		std::cout << "Input" << std::endl;
		this->pointer++;
		result = input();
	}
	return result;
}

int Parser::output(void)
{
	// <output> ::= [OUTPUT] <expression>
	if (this->tokens[pointer].type == "[OUTPUT]" && expression(pointer + 1) == 0)
	{
		return 0;
	}
    return 1;
}

int Parser::input(void)
{
	// <input> ::= [INPUT] [IDENTIFIER] <statement>
	if (this->tokens[this->pointer].type == "[IDENTIFIER]")
	{
		return 0;
	}
	return 1;
}

int Parser::function(void)
{
	// <function> ::= [FUNCTION] [IDENTIFIER] ( <function-parameter> [RETURNS] <primitive-type> <statement> [ENDFUNCTION]
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (this->tokens[this->pointer].type != "[IDENTIFIER]")
	{
		return 1;
	}
	this->pointer++;
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (this->tokens[this->pointer].type != "[LBRACKET]")
	{
		return 1;
	}
	this->pointer++;
	if (this->tokens[this->pointer].type == "[RBRACKET]") // function without parameters
	{
		std::cout << "Checking potential empty function" << std::endl;
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

int Parser::expression(int pointer)
{
	// <expression> ::= 
	std::cout << this->tokens[pointer].type << std::endl;
    if (primitive_literal() == 0)
	{
		this->pointer++;
		std::cout << "expression is " << this->tokens[pointer].value << std::endl;
		return 0;
	}
	if (primitive_literal() == 0)
	{
		std::cout << "Primitive literal" << std::endl;
		return 0;
	}
	return 1;
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
    if ((pointer + 2 < this->tokens.size()) && operator_(pointer + 1) == 0 && expression(pointer + 2) == 0)
    {
        return 0;
    }
    return 1;
}

int Parser::unary_expression(int pointer)
{
    if ((this->tokens[pointer].type == "[NOT]" || this->tokens[pointer].type == "[SUBTRACTION]") && expression(pointer + 1) == 0)
    {
        return 0;
    }
    return 1;
}
