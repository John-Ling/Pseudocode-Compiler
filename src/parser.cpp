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
        try
        {
            Node* statementNode = statement();
            this->nodes.push_back(statementNode);
        }
        catch(Parse_Error e)
        {
            throw Parse_Error();
        }
        catch(Reached_End_Of_File e)
        {
            break;
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

bool Parser::peek(std::string tokenType) // matches the next token with tokenType
{
    bool matched = false;
    if (this->pointer + 1 < this->tokens.size())
    {
        if (this->tokens[this->pointer + 1].type == tokenType)
        {
            matched = true;
        }
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

Node* Parser::statement(void)
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
    else if (match(Tokens::END_OF_FILE))
    {
        throw Reached_End_Of_File();
    }
    else
    {
        // attempt to generate a standalone expression node
        return expression();
    }
}

Node* Parser::primitive_literal(void)
{
    if (match(Tokens::INTEGER_LITERAL) ||
        match(Tokens::FLOAT_LITERAL)   ||
        match(Tokens::STRING_LITERAL)  ||
        match(Tokens::BOOLEAN_LITERAL))
    {
        return new Literal(this->tokens[this->pointer]);
    }
    throw Parse_Error();
}

Node* Parser::primitive_type(void)
{
    if (match(Tokens::INTEGER) ||
        match(Tokens::STRING)  ||
        match(Tokens::BOOLEAN) ||
        match(Tokens::FLOAT))
    {
        return new Primitive(this->tokens[this->pointer]);
    }
    throw Parse_Error();
}

Node* Parser::variable_declaration(void)
{
    // <variable-declaration> ::= [DECLARE] [IDENTIFIER] : <primitive-type>
    if (!match(Tokens::IDENTIFIER))
    {
        throw Parse_Error();
    }
    Token identifier = this->tokens[this->pointer];
    advance();

    if (!match(Tokens::COLON))
    {
        throw Parse_Error();
    }
    advance();

    try
    {
        Node* primitive = primitive_type();
        return new Variable_Declaration(identifier, primitive);
    }
    catch(Parse_Error e)
    {
        throw Parse_Error();
    }
}

int Parser::output(void)
{
    // <output> ::= [OUTPUT] <expression> (, <expression>)*
    Node* expressionNode = expression();
    
    if (expression() == 1)
    {
        return 1;
    }

    while (peek(Tokens::COMMA) && this->pointer != -1)
    {
        advance(); // reach comma
        advance(); // skip comma
        if (expression() == 1)
        {
            return 1;
        }
    }
    return 0;
}

int Parser::input(void)
{
    // <input> ::= [INPUT] [IDENTIFIER]
    if (!match(Tokens::IDENTIFIER))
    {
        return 1;
    }
    return 0;
}

int Parser::for_(void)
{
    // <for> ::= [FOR] [IDENTIFIER] [ASSIGNMENT] <expression> TO <expression> ([STEP] [INTEGER_LITERAL])? (<statement>)* [NEXT] [IDENTIFIER]
    if (!match(Tokens::IDENTIFIER))
    {
        return 1;
    }
    advance();
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
    if (!match(Tokens::TO))
    {
        return 1;
    }
    advance();
    if (expression() == 1)
    {
        return 1;
    }

    if (peek(Tokens::STEP))
    {
        advance();
        if (!match(Tokens::INTEGER_LITERAL))
        {
            return 1;
        }
    }
    advance();
    while (!match(Tokens::NEXT) && this->pointer != -1)
    {
        if (statement() == 1)
        {
            return 1;
        }
        advance();
    }
    advance();
    if (!match(Tokens::IDENTIFIER))
    {
        return 1;
    }
    return 0;
}

int Parser::if_(void)
{
    // <if> ::= [IF] <expression> [THEN] (<statement>)* [ENDIF]
    if (expression() == 1)
    {
        return 1;
    }
    advance();
    if (!match(Tokens::THEN))
    {
        return 1;
    }
    advance();
    while (!match(Tokens::ENDIF) && this->pointer != -1)
    {
        if (match(Tokens::ELSE))
        {
            std::cout << "Reached else block\n";
        }
        else if (statement() == 1)
        {
            return 1;
        }
        advance();
    }

    if (this->pointer == -1)
    {
        return 1;
    }
    return 0;
}

int Parser::else_(void)
{
    return 0;
}

int Parser::while_(void)
{
    // <while> ::= [WHILE] <expression> (<statement>)* [ENDWHILE]
    if (expression() == 1)
    {
        return 1;
    }
    advance();
    while (!match(Tokens::ENDWHILE) && this->pointer != -1)
    {
        if (statement() == 1)
        {
            return 1;
        }
        advance();
    }

    if (this->pointer == -1)
    {
        return 1;
    }
    return 0;
}

Node* Parser::return_(void)
{
    // <return> ::= [RETURN] <expression>
    Node* expressionNode = expression();
    return new Return(expressionNode); 
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

    int functionArgumentsCount = 0;
    while (!match(Tokens::RBRACKET) && this->pointer != -1)
    {
        if (functionArgumentsCount > 0)  // one comma to separate each function argument
        {
            if (!match(Tokens::COMMA))
            {
                return 1;
            }
            advance();
        }

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

        functionArgumentsCount++;
    }

    if (this->pointer == -1)
    {
        return 1;
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

    while (!match(Tokens::ENDFUNCTION) && this->pointer != -1)
    {
        if (statement() == 1)
        {
            return 1;
        }
        advance();
    }
    if (this->pointer == -1)
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

// EXPRESSIONS //
// <expression> ::= <logical-comparison>
// <logical-comparison> ::= <equality> (([AND] | [OR]) <equality>)*
// <equality> ::= <comparison> ( ( <> | = )  <comparison> )*
// <comparison> ::= <term> ( ( > | >= | < | <= ) <term> )*
// <term> ::= <factor> ( ( - | + ) <factor> )*
// <factor> ::= <unary> ( ( * | / ) <unary> )*
// <unary> ::= ([NOT] | - ) <unary> | <primary>
// <primary ::= <primitive-literal> | [IDENTIFIER] ( <function-call-parameter> ) | [IDENTIFIER] | ( <expression> )
// <function-call-parameter> ::= <expression> , <function-call-parameter> | <expression>

Node* Parser::expression(void) // when the parser finishes an expression the pointer will be located at the token that finished the expression
{
    //<expression> ::= <equality>
    return logical_comparison();
}

Node* Parser::logical_comparison(void)
{
    Node* expression = equality();

    while (peek(Tokens::AND) || peek(Tokens::OR))
    {
        advance();
        Token operation = this->tokens[this->pointer];
        advance();
        Node* rightExpression = equality();
        expression = new Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node* Parser::equality(void)
{
    Node* expression = numerical_comparison();

    while (peek(Tokens::EQUAL) || peek(Tokens::NOT_EQUAL))
    {
        advance();
        Token operation = this->tokens[this->pointer];
        advance();
        Node* rightExpression = numerical_comparison();
        expression = new Binary_Expression(expression, operation, rightExpression);
    }

    return expression;
}

Node* Parser::numerical_comparison(void)
{
    Node* expression = term();

    while (peek(Tokens::GREATER) || peek(Tokens::GREATER_EQUAL) || peek(Tokens::LESSER) || peek(Tokens::LESSER_EQUAL))
    {
        advance();
        Token operation = this->tokens[this->pointer];
        advance();
        Node* rightExpression = term();
        expression = new Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node* Parser::term(void)
{
    Node* expression = factor();

    while (peek(Tokens::ADDITION) || peek(Tokens::SUBTRACTION))
    {
        advance(); // skip over addition and subtraction to form next expression
        Token operation = this->tokens[this->pointer];
        advance();
        Node* rightExpression = factor();
        expression = new Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node* Parser::factor(void)
{
    Node* expression = unary();

    while (peek(Tokens::MULTIPLICATION) || peek(Tokens::DIVISION))
    {
        advance();
        Token operation = this->tokens[this->pointer];
        advance();
        Node* rightExpression = unary();
        expression = new Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node* Parser::unary(void)
{
    if (match(Tokens::SUBTRACTION) || match(Tokens::NOT))
    {
        Token unaryOperator = this->tokens[this->pointer];
        advance();
        return new Unary_Expression(unary(), unaryOperator); // create and return unary expression
    }
    else
    {
        return primary();
    }
}

Node* Parser::primary(void)
{
    if (primitive_literal() == 0)
    {
        return new Literal(this->tokens[this->pointer]);
    }
    else if (match(Tokens::IDENTIFIER))
    {
        advance();
        if (match(Tokens::LBRACKET))
        {
            advance();
            return new function_call_parameter();
        }

        try
        {
            return new variable_assignment();
        }
        catch(Parse_Error e)
        {
            return new Literal(this->tokens[this->pointer]);
        }
    }
    else if (match(Tokens::LBRACKET))
    {
        advance();
        if (expression() == 1)
        {
            return 1;
        }
        advance();
        if (match(Tokens::RBRACKET))
        {
            return 0;
        }
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
        if (expression() == 1)
        {
            return 1;
        }
        advance();
        if (match(Tokens::RBRACKET))
        {
            return 0;
        }
        else if (!match(Tokens::COMMA))
        {
            return 1;
        }
        advance();
        return function_call_parameter();
    }
}

Node* Parser::variable_assignment(void)
{
    // <variable-assignment> ::= [IDENTIFIER] [ASSIGNMENT] <expression>
    if (!match(Tokens::ASSIGNMENT))
    {
        throw Parse_Error();
    }

    advance();
    return 0;
}

// END EXPRESSION //
