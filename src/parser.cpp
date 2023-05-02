#include "parser.h"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->pointer = 0;
}

// main 
int Parser::parse_tokens(void)
{
    while (this->pointer != -1 && this->tokens[this->pointer].type != Tokens::END_OF_FILE)
    {
        if (compare(Tokens::END_OF_LINE))
        {
            advance();
            continue;
        }

        try
        {
            Node* statementNode = statement();
            this->nodes.push_back(statementNode);
        }
        catch(Error &e)
        {
            e.display_problem();
            exit(1);
        }
        advance();
    }
    return 0;
}

// helper functions

// checks if the current token matches argument tokenType
bool Parser::compare(std::string tokenType)
{
    bool matched = false;

    if (this->pointer != -1 && this->pointer < (int)this->tokens.size() && this->tokens[this->pointer].type == tokenType)
    {
        matched = true;
    }
    return matched;
}

// similar to compare() but throws a Unexpected_Parsing_Input() if match unsuccessful
void Parser::match(std::string tokenType) 
{
    if (!compare(tokenType)) 
    { 
        throw Unexpected_Parsing_Input(tokenType, this->tokens[this->pointer].value); 
    }
    return;
}

// matches the next token with tokenType and advances if matched successfully
bool Parser::peek(std::string tokenType)
{
    bool matched = false;
    if (this->pointer + 1 < (int)this->tokens.size())
    {
        if (this->tokens[this->pointer + 1].type == tokenType)
        {
            matched = true;
        }
    }
    return matched;
}

// Increments the pointer attribute if possible setting it to -1 if not. If the current token is end of line (EOL) After incrementing it will call itself to skip it
void Parser::advance(void)
{
    if (this->pointer == -1 || this->pointer + 1 >= (int)this->tokens.size())
    {
        this->pointer = -1;
    }
    else
    {
        this->pointer++;
        if (compare(Tokens::END_OF_LINE)) // if reached a end of line token skip over it
        {
            advance(); // run advance() to check if increment is still possible rather than just settings pointer to ++
        }
    }
    return;
}

void Parser::double_advance(void) // made this because I didn't like putting advance() twice lmao
{
    advance();
    advance();
}


// ast node functions

Node* Parser::statement(void)
{
    Node statement;
    if (compare(Tokens::OUTPUT))
    {
        // generate an output node
        advance();
        return output();
    }
    else if (compare(Tokens::FUNCTION))
    {
        // generate a function declaration node
        advance();
        return function();
    }
    else if (compare(Tokens::DECLARE))
    {
        // generate a variable declaration node
        advance();
        return variable_declaration();
    }
    else if (compare(Tokens::INPUT))
    {
        // generate an input node
        advance();
        return input();
    }
    else if (compare(Tokens::FOR))
    {
        advance();
        return for_();
    }
    else if (compare(Tokens::RETURN))
    {
        advance();
        return return_();
    }
    else if (compare(Tokens::WHILE))
    {
        advance();
        return while_();
    }
    else if (compare(Tokens::IF))
    {
        advance();
        return if_();
    }
    else
    {
        // attempt to generate a standalone expression node
        return expression();
    }
}

// generates a node to represent the many statements inside "structures" such as if statements, loops or functions
std::vector<Node*> Parser::block_statement(std::vector<std::string> terminators)
{
    auto compare_terminators = [this, terminators](void)
    {
        for (unsigned int i = 0; i < terminators.size(); i++)
        {
            if (compare(terminators[i]))
            {
                return true;
            }
        }
        return false;
    };

    std::vector<Node*> statements;
    while (!compare_terminators() && this->pointer != -1)
    {
        statements.push_back(statement());
        advance();
    }
    
    if (this->pointer == -1)
    {
        throw Missed_Terminating_Token(terminators[0]);
    }

    return statements;
}

Node* Parser::primitive_literal(void)
{
    const std::vector<std::string> LITERALS = {Tokens::INTEGER_LITERAL, Tokens::FLOAT_LITERAL, Tokens::STRING_LITERAL, Tokens::BOOLEAN_LITERAL};
    for (int i = 0; i < (int)LITERALS.size(); i++)
    {
        if (compare(LITERALS[i]))
        {
            return new Literal(this->tokens[this->pointer]);
        }
    }
    throw Generic_Error();
}

Node* Parser::primitive_type(void)
{
    const std::vector<std::string> PRIMITIVES = {Tokens::INTEGER, Tokens::STRING, Tokens::BOOLEAN, Tokens::FLOAT};
    for (int i = 0; i < (int)PRIMITIVES.size(); i++)
    {
        if (compare(PRIMITIVES[i]))
        {
            return new Primitive(this->tokens[this->pointer]);
        }
    }
    throw Generic_Error();
}

// <variable-declaration> ::= [DECLARE] [IDENTIFIER] : <primitive-type>
Node* Parser::variable_declaration(void)
{    
    match(Tokens::IDENTIFIER);
    Node* identifier = new Identifier(this->tokens[this->pointer]);
    advance();
    match(Tokens::COLON);
    advance();

    Node* primitive = primitive_type();
    return new Variable_Declaration(identifier, primitive);
}

// <output> ::= [OUTPUT] <expression> (, <expression>)*
Node* Parser::output(void)
{   
    std::vector<Node*> expressions = {expression()};
    while (peek(Tokens::COMMA) && this->pointer != -1)
    {
        double_advance();
        Node* expressionNode = expression();
        expressions.push_back(expressionNode);
    }
    return new Output(expressions);
}

// <input> ::= [INPUT] [IDENTIFIER]
Node* Parser::input(void)
{
    match(Tokens::IDENTIFIER);
    Node* identifier = new Identifier(this->tokens[this->pointer]);
    return new Input(identifier);
}

Node* Parser::for_(void)
{
    // <for> ::= [FOR] [IDENTIFIER] [ASSIGNMENT] <expression> TO <expression> ([STEP] [INTEGER_LITERAL])? (<statement>)* [NEXT] [IDENTIFIER]
    bool stepExpressionPresent = false;

    match(Tokens::IDENTIFIER);
    Node* indexVariable = new Identifier(this->tokens[this->pointer]);
    advance();
    match(Tokens::ASSIGNMENT);
    advance();
    Node* startExpression = expression();
    advance();
    match(Tokens::TO);
    advance();
    Node* endExpression = expression();
    advance();
    Node* stepExpression;

    if (compare(Tokens::STEP)) // optional step keyword 
    {
        advance();
        stepExpression = expression();
        advance();
    }

    const std::vector<std::string> TERMINATORS = {Tokens::NEXT};
    std::vector<Node*> statements = block_statement(TERMINATORS);
    advance();
    match(Tokens::IDENTIFIER);

    if (stepExpressionPresent)
    {
        return new For(startExpression, endExpression, stepExpression, indexVariable, statements);
    }
    return new For(startExpression, endExpression, indexVariable, statements);
}

// <if> ::= [IF] <expression> [THEN] (<statement>)* ([ENDIF] | [ELSE] <else>)
Node* Parser::if_(void)
{
    Node* condition = expression();
    advance();
    match(Tokens::THEN);
    advance();

    const std::vector<std::string> TERMINATORS = {Tokens::ENDIF, Tokens::ELSE};
    std::vector<Node*> statements = block_statement(TERMINATORS);
    
    if (compare(Tokens::ELSE)) 
    {
        advance();
        Node* elseNode = else_();
        return new If(condition, statements, elseNode);
    }
    else
    {
        return new If(condition, statements);
    }
}

// <else> ::= (<statement>)* [ENDIF]
Node* Parser::else_(void)
{
    const std::vector<std::string> TERMINATORS = {Tokens::ENDIF};
    std::vector<Node*> statements = block_statement(TERMINATORS);
    return new Else(statements);
}

// <while> ::= [WHILE] <expression> (<statement>)* [ENDWHILE]
Node* Parser::while_(void)
{
    Node* condition = expression();
    advance();
    const std::vector<std::string> TERMINATORS = {Tokens::ENDWHILE};
    std::vector<Node*> statements = block_statement(TERMINATORS);
    return new While(condition, statements);
}

Node* Parser::return_(void)
{
    // <return> ::= [RETURN] <expression>
    Node* expressionNode = expression();
    return new Return(expressionNode); 
}

// <function> ::= [FUNCTION] [IDENTIFIER] ( <function-parameter> [RETURNS] <primitive-type> (<statement>)* [ENDFUNCTION]
Node* Parser::function(void)
{
    match(Tokens::IDENTIFIER);
    Node* functionName = new Identifier(this->tokens[this->pointer]);
    advance();
    match(Tokens::LBRACKET);
    advance();

    Node* arguments = function_arguments();
    
    advance();
    match(Tokens::RETURNS);
    advance();

    Node* returnType = primitive_type();
    advance();

    const std::vector<std::string> TERMINATORS = {Tokens::ENDFUNCTION};
    std::vector<Node*> statements = block_statement(TERMINATORS);
    return new Function(functionName, returnType, arguments, statements);
}

Node* Parser::function_arguments(void)
{
    unsigned int argumentCount = 0;
    Function_Arguments* arguments = new Function_Arguments;
    while (!compare(Tokens::RBRACKET) && this->pointer != -1)
    {
        if (argumentCount > 0)  // one comma to separate each function argument
        {
            match(Tokens::COMMA);
            advance();
        }

        match(Tokens::IDENTIFIER);
        Node* identifier = new Identifier(this->tokens[this->pointer]);
        advance();
        match(Tokens::COLON);
        advance();
        Node* type = primitive_type();
        advance();
        
        arguments->add_argument(identifier, type);
        argumentCount++;
    }

    if (this->pointer == -1) 
    { 
        throw Missed_Terminating_Token(Keywords::LBRACKET); 
    }
    return arguments;
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

// important detail: when the parser finishes an expression the pointer will be located at the token that finished the expression

Node* Parser::expression(void)
{ 
    //<expression> ::= <equality>
    return logical_comparison();
}

Node* Parser::logical_comparison(void) // most expression nodes are in this structure
{
    Node* expression = equality(); // form left side of expression

    while (peek(Tokens::AND) || peek(Tokens::OR)) // check if the respective operator is next
    {
        advance();
        Token operation = this->tokens[this->pointer]; // collect operator
        advance();
        Node* rightExpression = equality(); // form lower precedence expression 
        expression = new Binary_Expression(expression, operation, rightExpression); // form node
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
        advance();
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
    if (compare(Tokens::SUBTRACTION) || compare(Tokens::NOT))
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
    if (compare(Tokens::IDENTIFIER))
    {
        auto peek_operators = [this](void)
        {
            for (unsigned int i = 0; i < Tokens::OPERATORS.size(); i++) // OPERATORS can be found in constants.h
            {
                if (peek(Tokens::OPERATORS[i]))
                {
                    return true;
                }
            }
            return false;
        };

        Node* identifier = new Identifier(this->tokens[this->pointer]);

        if (peek(Tokens::LBRACKET)) // try form function call node
        {
            double_advance();
            Node* arguments = function_call_arguments();
            return new Function_Call(identifier, arguments);
        }

        if (peek(Tokens::ASSIGNMENT)) // try form variable assignment node
        {
            double_advance();
            Node* assignmentExpression = expression();
            return new Variable_Assignment(identifier, assignmentExpression);
        }
        else if (peek(Tokens::END_OF_LINE) || peek_operators())
        {
            return identifier; // return a regular identifier
        }
        
        throw Generic_Error();
    }
    else if (compare(Tokens::LBRACKET)) // attempt to form bracketed expression
    {
        advance();
        Node* expressionNode = expression();
        advance();
        match(Tokens::RBRACKET);
        return expressionNode;
    }
    else
    {
        return primitive_literal();
    }
}

Node* Parser::function_call_arguments(void)
{
    // <function-call-parameter> ::= <expression> , <function-call-parameter> | )
    Call_Arguments* arguments = new Call_Arguments;
    arguments->add_argument(expression());
    advance();
    while (!(compare(Tokens::COMMA) || compare(Tokens::RBRACKET)) && this->pointer != -1)
    {
        arguments->add_argument(expression());
        advance();
    }
    return arguments;
}

// END EXPRESSION //