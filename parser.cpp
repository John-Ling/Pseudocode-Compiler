#include "parser.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->pointer = 0;
}

int Parser::parse_tokens(void)
{
    while (this->pointer != -1)
	{
		if (statement() == 1)
		{
			return 1;
		}
		advance();
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
	 
    if (match(Tokens::OUTPUT))
    {
		// generate an output node
		advance();
		return output();
	}
	else if (match(Tokens::FUNCTION))
	{
		// generate a function declaration node
		advance();
		return function();
	}
	else if (match(Tokens::DECLARE))
	{
		// generate a variable declaration node
		advance();
		return variable_declaration();
	}
	else if (match(Tokens::INPUT))
	{
		// generate an input node
		advance();
		return input();
	}
	else if (match(Tokens::FOR))
	{
		advance();
		return for_();
	}
	else if (match(Tokens::IDENTIFIER))
	{
		advance();
		return variable_assignment();
	}
	else if (match(Tokens::RETURN))
	{
		advance();
		return return_();
	}
	else if (match(Tokens::WHILE))
	{
		advance();
		return while_();
	}
	else if (match(Tokens::IF))
	{
		advance();
		return if_();
	}
	else if (match(Tokens::ELSE))
	{
		advance();
		return else_();
	}
	else if (match(Tokens::ENDFUNCTION))
	{ 
		return 0;
	}
	else if (match(Tokens::ENDWHILE))
	{
		return 0;
	}
	else if (match(Tokens::ENDIF))
	{
		return 0;
	}
	else if (match(Tokens::END_OF_FILE))
	{
		return 0;
	}
	else
	{
		// attempt to generate a standalone expression node
		 
		return expression();
	}
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

int Parser::variable_declaration(void)
{
	// <variable-declaration> ::= [DECLARE] [IDENTIFIER] : <primitive-type> <statement>
	if (!match(Tokens::IDENTIFIER))
	{
		return 1;
	}
	advance();
	if (!match(Tokens::COLON))
	{
		return 1;
	}
	advance();
	if (primitive_type() == 1)
	{
		return 1;
	}
	 
	advance();
	if (statement() == 1)
	{
		return 1;
	}
	return 0;
}

int Parser::variable_assignment(void)
{
	// <variable-assignment> ::= [IDENTIFIER] [ASSIGNMENT] <expression> <statement>
	if (!match(Tokens::ASSIGNMENT))
	{
		return 1;
	}
	advance();
	if (expression() == 1)
	{
		return 1;
	}
	 
	advance();
	if (statement() == 1)
	{
		return 1;
	}
	return 0;
}

int Parser::output(void)
{
	// <output> ::= [OUTPUT] <expression> <statement>
	if (expression() == 1)
	{
		return 1;
	}
	if (statement() == 1)
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
	advance();
	if (statement() == 1)
	{
		return 1;
	}
	return 0;
}

int Parser::for_(void)
{
	return 0;
}

int Parser::if_(void)
{
	return 0;
}

int Parser::else_(void)
{
	return 0;
}

int Parser::while_(void)
{
	return 0;
}

int Parser::return_(void)
{
	// <return> ::= [RETURN] <expression>
	if (expression() == 1)
	{
		return 1;
	}
	 
	advance();
	 
	if (statement() == 1)
	{
		return 1;
	}
	return 0;
}

int Parser::function(void)
{
	// <function> ::= [FUNCTION] [IDENTIFIER] ( <function-parameter> [RETURNS] <primitive-type> <statement> [ENDFUNCTION]
	 
	if (!match(Tokens::IDENTIFIER))
	{
		return 1;
	}
	advance();
	 
	if (!match(Tokens::LBRACKET))
	{
		return 1;
	}
	advance();
	if (match(Tokens::RBRACKET)) // function without parameters
	{
		advance();
		if (!match(Tokens::RETURNS))
		{
			return 1;
		}
		advance();
		if (primitive_type() == 1)
		{
			return 1;
		}
		return 0; 
	}
	else
	{
		if (!match(Tokens::IDENTIFIER))
		{
			return 1;
		}
		advance();
		if (!match(Tokens::COLON))
		{
			return 1;
		}
		advance();
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
	if (!match(Tokens::RETURNS))
	{
		return 1;
	}
	advance();
	if (primitive_type() == 1)
	{
		return 1;
	}
	advance();
	if (statement() == 1)
	{
		return 1;
	}
	 
	return 0;
}

int Parser::function_parameter(void)
{
	// <function-parameter> ::= ) | , [IDENTIFIER] : <primitive-literal> <function-parameter>
	 
	if (this->tokens[this->pointer].type == "[RBRACKET]")
	{
		 
		return 0;
	}
	else if (this->tokens[this->pointer].type == "[COMMA]")
	{
		advance();
		 
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
		advance();
		return function_parameter(); // recursively check if there are any more valid arguments and return the correct status integer
	}
	 
	return 1;
}

int Parser::function_call_parameter(void)
{
	// <function-call-parameter> ::= <expression> , <function-call-parameter> | )
	if (match(Tokens::RBRACKET))
	{
		 
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

// EXPRESSION //
// <expression> ::= <equality>
// <equality> ::= <comparison> ( ( <> | = )  <comparison> )*
// <comparison> ::= <term> ( ( > | >= | < | <= ) <term> )*
// <term> ::= <factor> ( ( - | + ) <factor> )*
// <factor> ::= <unary> ( ( * | / ) <unary> )*
// <unary> ::= ([NOT] | - ) <unary> | <primary>
// <primary ::= <primitive-literal> | [IDENTIFIER] ( <function-call-parameter> ) | [IDENTIFIER] | ( <expression> )
// <function-call-parameter> ::= <expression> , <function-call-parameter> | <expression>

int Parser::expression(void)
{
	//<expression> ::= <equality>
	 
	if (equality() == 1) 
	{
		return 1;
	}
	return 0;
}

int Parser::equality(void) 
{
	 
	if (comparison() == 1)
	{
		return 1;
	}
	if (this->pointer == -1)
	{
		return 0;
	}
	while (match(Tokens::NOT_EQUAL) || match(Tokens::EQUAL))
	{
		advance();
		if (comparison() == 1)
		{
			return 1;
		}
	}
	// add equality node
	return 0;
}

int Parser::comparison(void)
{
	 
	if (term() == 1)
	{
		return 1;
	}
	if (this->pointer == -1)
	{
		return 0;
	}
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
	 
	if (factor() == 1)
	{
		return 1;
	}
	 
	if (this->pointer == -1)
	{
		return 0;
	}
	while (match(Tokens::ADDITION) || match(Tokens::SUBTRACTION))
	{
		advance();
		if (factor() == 1)
		{
			return 1;
		}
		 
	}
	// add term node
	return 0;
}

int Parser::factor(void)
{
	 
	if (unary() == 1)
	{
		return 1;	
	}
	 
	if (this->pointer == -1)
	{
		return 0;
	}
	while (match(Tokens::MULTIPLICATION) || match(Tokens::DIVISION)) 
	{
		advance();
		if (unary() == 1)
		{
			return 1;
		}
	}
	// add factor node
	return 0;
}

int Parser::unary(void)
{
	 
	if (match(Tokens::SUBTRACTION) || match(Tokens::NOT))
	{
		advance();
		// add unary node
		return unary();
	}
	else if (primary() == 0)
	{
		advance();
		return 0;
	}
	else
	{
		return 1;
	}
}

int Parser::primary(void)
{
	 
	if (primitive_literal() == 0)
	{
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
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if (match(Tokens::LBRACKET))
	{
		advance();
		if (expression() == 1)
		{
			return 1;
		}
		 
		 
		if (match(Tokens::RBRACKET))
		{
			return 0;
		}
	}
	return 1;
}

// END EXPRESSION // 
