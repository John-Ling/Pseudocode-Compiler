#include "parser.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
	this->errorOccurred =
    this->pointer = 0;
}

int Parser::parse_tokens(void)
{
    while (this->pointer < this->tokens.size() && !errorOccurred)
	{
		std::cout << "Looking at token " << this->tokens[this->pointer].type << std::endl;
		int result = statement(); // parse token pointed to by this->pointer
		if (result == 1)
		{
			std::cout << "Failed" << std::endl;
			return 1;
		}
		advance();
	}
	if (errorOccurred)
	{
		return 1;
	}
	return 0;
}

bool Parser::match(std::string tokenType)
{
	bool matched = false;
	if (this->pointer != -1 && this->pointer < this->tokens.size() && this->tokens[this->pointer].type == tokenType)
	{
		matched = true;
	}
	return matched;
}

void Parser::advance(void)
{
	if (this->pointer == -1 || this->pointer + 1 >= this->tokens.size())
	{
		this->errorOccurred = true;
		this->pointer = -1;
	}
	else
	{
		this->pointer++;
	}
	return;
}

int Parser::statement(void)
{
	int result = 1;
	std::cout << this->tokens[this->pointer].type << std::endl;
    if (match(Tokens::OUTPUT))
    {
		// generate an output node
		std::cout << "Going to function OUTPUT" << std::endl;
		advance();
		result = output();
	}
	else if (match(Tokens::FUNCTION))
	{
		// generate a function declaration node
		std::cout << "Function" << std::endl;
		advance();
		result = function();
	}
	else if (match(Tokens::DECLARE))
	{
		// generate a variable declaration node
		std::cout << "Declare" << std::endl;
		advance();
		result = variable_declaration();
	}
	else if (match(Tokens::INPUT))
	{
		// generate an input node
		std::cout << "Input" << std::endl;
		advance();
		result = input();
	}
	else
	{
		// attempt to generate a standalone expression node
		std::cout << "Calling function expression\n";
		result = expression();
	}
	return result;
}

int Parser::expression(void)
{
	//<expression> ::= <equality>
	std::cout << "Checking expression\n";
	std::cout << this->tokens[this->pointer].type << '\n';
	if (equality() == 1) 
	{
		return 1;
	}
	std::cout << "Valid Expression\n";
	return 0;
}

int Parser::equality(void) 
{
	std::cout << "Checking equality\n"; 
	std::cout << this->tokens[this->pointer].type << '\n';
	if (comparison() == 1)
	{
		return 1;
	}
	std::cout << "Checking Not Equal or Equal\n";
	while (match(Tokens::NOT_EQUAL) || match(Tokens::EQUAL))
	{
		advance();
		if (comparison() == 1)
		{
			std::cout << "Failed\n";
			return 1;
		}
	}
	// add equality node
	return 0;
}

int Parser::comparison(void)
{
	std::cout << "Checking Comparison\n";
	std::cout << this->tokens[this->pointer].type << '\n';
	if (term() == 1)
	{
		return 1;
	}
	std::cout << "Checking comparison operators\n";
	while (match(Tokens::LESSER)       ||
		   match(Tokens::LESSER_EQUAL) ||
		   match(Tokens::GREATER)      ||
		   match(Tokens::GREATER_EQUAL))
	{
		advance();
		if (term() == 1)
		{
			return 1;
		}
	}
	// add comparison node
	return 0;
}

int Parser::term(void) 
{
	std::cout << "Checking term\n";
	std::cout << this->tokens[this->pointer].type << '\n';
	if (factor() == 1)
	{
		return 1;
	}
	std::cout << "Checking for addition or subtraction\n";
	std::cout << this->pointer << '\n';
	std::cout << this->tokens[this->pointer].type << '\n';
	while (match(Tokens::ADDITION) || match(Tokens::SUBTRACTION))
	{
		advance();
		if (factor() == 1)
		{
			return 1;
		}
		std::cout << this->pointer << '\n';
	}
	// add term node
	return 0;
}

int Parser::factor(void)
{
	std::cout << "Checking factor\n";
	std::cout << this->tokens[this->pointer].type << '\n';
	if (unary() == 1)
	{
		return 1;	
	}
	std::cout << "After unary \n";
	std::cout << this->pointer << '\n';
	while (match(Tokens::MULTIPLICATION) || match(Tokens::DIVISION)) 
	{
		std::cout << "Found Multiplication or Division\n";
		advance();
		if (unary() == 1)
		{
			return 1;
		}
	}
	// add factor node
	std::cout << "Completed Factor\n";
	return 0;
}

int Parser::unary(void)
{
	std::cout << "Checking unary stuff\n";
	std::cout << this->tokens[this->pointer].type << '\n';
	if (match(Tokens::SUBTRACTION) || match(Tokens::NOT))
	{
		advance();
		// add unary node
		return unary();
	}
	else if (primary() == 0)
	{
		advance();
		std::cout << "Unary Complete\n";
		return 0;
	}
	else
	{
		std::cout << "Unary failed successfully\n";
		return 1;
	}
}

int Parser::primary(void)
{
	std::cout << "Reached primary\n";
	std::cout << this->pointer << '\n';
	if (primitive_literal() == 0)
	{
		std::cout << "Found primitive literal " << this->tokens[this->pointer].value << '\n';
		return 0;
	}
	else if (match(Tokens::IDENTIFIER))
	{
		advance();
		if (match(Tokens::LBRACKET))
		{
			advance();
			if (function_parameter() == 0)
			{
				std::cout << "Function Call\n";
				return 0;
			}
		}
		else
		{
			std::cout << "Identifier\n";
			return 0;
		}
	}
	else if (match(Tokens::LBRACKET))
	{
		std::cout << "Bracketed expression\n";
		advance();
		std::cout << "Before:" << this->pointer << '\n';
		if (expression() == 1)
		{
			return 1;
		}
		std::cout << "After: " << this->pointer << '\n';
		std::cout << this->pointer << std::endl;
		std::cout << "Checking for final bracket\n";
		std::cout << this->tokens[this->pointer].type << '\n';
		if (match(Tokens::RBRACKET))
		{
			std::cout << "Completed bracketed expression\n";
			return 0;
		}
		else
		{
			std::cout << "Can't find completing bracket\n";
		}
	}
	return 1;
}

int Parser::primitive_literal(void)
{

	if (match(Tokens::INTEGER_LITERAL) ||
		match(Tokens::FLOAT_LITERAL)   ||
		match(Tokens::STRING_LITERAL)  || 
		match(Tokens::BOOLEAN_LITERAL))
	{
		return 0;        
	}
	return 1;
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
	if (!match(Tokens::IDENTIFIER))
	{
		return 1;
	}
	return 0;
}

int Parser::function(void)
{
	// <function> ::= [FUNCTION] [IDENTIFIER] ( <function-parameter> [RETURNS] <primitive-type> <statement> [ENDFUNCTION]
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (!match(Tokens::IDENTIFIER))
	{
		return 1;
	}
	advance();
	std::cout << this->tokens[this->pointer].type << std::endl;
	if (!match(Tokens::LBRACKET))
	{
		return 1;
	}
	advance();
	if (match(Tokens::RBRACKET)) // function without parameters
	{
		std::cout << "Checking potential empty function" << std::endl;
		advance();
		if (!match(Tokens::RETURNS))
		{
			return 1;
		}
		advance();
		if (!primitive_type())
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
		if (!match(Tokens::IDENTIFIER))
		{
			return 1;
		}
		advance();
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (!match(Tokens::COLON))
		{
			return 1;
		}
		advance();
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (primitive_type() == 1)
		{
			return 1;
		}
		advance();
		if (function_parameter() == 1)
		{
			return 1;
		}
	}
	advance();
	if (this->tokens[this->pointer].type != "[RETURNS]")
	{
		return 1;
	}
	advance();
	if (primitive_type() == 1)
	{
		return 1;
	}
	std::cout << "Valid function" << std::endl;
	return 0;
}

int Parser::function_call_parameter(void)
{
	// <function-call-parameter> ::= <expression> , <function-call-parameter> | )
	if (match(Tokens::RBRACKET))
	{
		std::cout << "Completed function call parameter\n";
		return 0;
	}
	else
	{
		advance();
		if (expression() == 1)
		{
			return 1;
		}
		advance();
		if (match(Tokens::COMMA) == 1)
		{
			return 1;
		}
		return function_call_parameter();
	}
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
		advance();
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (this->tokens[this->pointer].type != "[IDENTIFIER]")
		{
			return 1;
		}
		advance();
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (this->tokens[this->pointer].type != "[COLON]")
		{
			return 1;
		}
		advance();
		std::cout << this->tokens[this->pointer].type << std::endl;
		if (primitive_type() == 1)
		{
			std::cout << "Failed literal test in function_parameter()" << std::endl;
			return 1;
		}
		advance();
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
	advance();
	if (this->tokens[this->pointer].type != "[COLON]")
	{
		return 1;
	}
	advance();
	if (primitive_type() == 1)
	{
		return 1;
	}
	std::cout << "valid declaration" << std::endl;
	return 0;
}

int Parser::primitive_type(void)
{
	if (match(Tokens::INTEGER) ||
		match(Tokens::STRING)  || 
		match(Tokens::BOOLEAN) || 
		match(Tokens::FLOAT))
	{
		return 0;
	}
	return 1;
}