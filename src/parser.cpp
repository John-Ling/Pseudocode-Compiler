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
        if (this->tokens[this->pointer].type == Tokens::END_OF_LINE)
        {
            std::cout << "Reached end of line\n";
            advance();
            continue;
        }

        try
        {
            Node statementNode = statement();
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
bool Parser::compare(std::string tokenType)
{
    bool matched = false;
    if (this->pointer != -1 && this->pointer < (int)this->tokens.size() && this->tokens[this->pointer].type == tokenType)
    {
        matched = true;
    }
    return matched;
}

void Parser::match(std::string tokenType) // similar to compare() but throws a Unexpected_Parsing_Input() if match unsuccessful
{
    if (!compare(tokenType)) { throw Unexpected_Parsing_Input(tokenType, this->tokens[this->pointer].value); }
    return;
}

bool Parser::peek(std::string tokenType) // matches the next token with tokenType and advances if matched successfully
{
    bool matched = false;
    if (this->pointer + 1 < (int)this->tokens.size())
    {
        if (this->tokens[this->pointer + 1].type == tokenType)
        {
            matched = true;
        }
    }

    if (matched)
    {
        advance();
    }
    return matched;
}

void Parser::advance(void)
{
    if (this->pointer == -1 || this->pointer + 1 >= (int)this->tokens.size())
    {
        this->pointer = -1;
    }
    else
    {
        this->pointer++;
    }
    return;
}

// ast node functions

Node Parser::statement(void)
{
    Node statement;
    std::cout << "Token into statement(): " << this->tokens[this->pointer].type << '\n';
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

Literal Parser::primitive_literal(void)
{
    const std::vector<std::string> LITERALS = {Tokens::INTEGER_LITERAL, Tokens::FLOAT_LITERAL, Tokens::STRING_LITERAL, Tokens::BOOLEAN_LITERAL};
    for (int i = 0; i < (int)LITERALS.size(); i++)
    {
        if (compare(LITERALS[i]))
        {
            return Literal(this->tokens[this->pointer]);
        }
    }
    throw Generic_Error();
}

Primitive Parser::primitive_type(void)
{
    const std::vector<std::string> PRIMITIVES = {Tokens::INTEGER, Tokens::STRING, Tokens::BOOLEAN, Tokens::FLOAT};
    for (int i = 0; i < (int)PRIMITIVES.size(); i++)
    {
        if (compare(PRIMITIVES[i]))
        {
            return Primitive(this->tokens[this->pointer]);
        }
    }
    throw Generic_Error();
}

Variable_Declaration Parser::variable_declaration(void)
{
    // <variable-declaration> ::= [DECLARE] [IDENTIFIER] : <primitive-type>
    match(Tokens::IDENTIFIER);
    Identifier identifier = Identifier(this->tokens[this->pointer]);
    advance();
    match(Tokens::COLON);
    advance();

    Primitive primitive = primitive_type();
    return Variable_Declaration(identifier, primitive);
}

Output Parser::output(void)
{
    // <output> ::= [OUTPUT] <expression> (, <expression>)*
    Node expressionNode = expression();
    std::vector<Node> expressions = {expressionNode};
    while (peek(Tokens::COMMA) && this->pointer != -1)
    {
        advance(); // skip comma
        Node expressionNode = expression();
        expressions.push_back(expressionNode);
    }
    return Output(expressions);
}

Input Parser::input(void)
{
    // <input> ::= [INPUT] [IDENTIFIER]
    match(Tokens::IDENTIFIER);
    Identifier identifier = Identifier(this->tokens[this->pointer]);
    return Input(identifier);
}

For Parser::for_(void)
{
    // <for> ::= [FOR] [IDENTIFIER] [ASSIGNMENT] <expression> TO <expression> ([STEP] [INTEGER_LITERAL])? (<statement>)* [NEXT] [IDENTIFIER]
    match(Tokens::IDENTIFIER);
    Identifier identifier = Identifier(this->tokens[this->pointer]);
    advance();
    match(Tokens::ASSIGNMENT);
    advance();
    Node startExpression = expression();
    advance();
    match(Tokens::TO);
    advance();
    Node endExpression = expression();
    advance();
    Node* stepExpression = NULL;

    if (compare(Tokens::STEP)) // optional step keyword 
    {
        advance();
        Node expressionNode = expression();
        stepExpression = &expressionNode;
        advance();
    }

    std::vector<Node> statements;
    while (!compare(Tokens::NEXT) && this->pointer != -1)
    {
        statements.push_back(statement());
        advance();
    }

    if (this->pointer == -1) { throw Missed_Terminating_Token(Keywords::NEXT); }

    advance();
    match(Tokens::IDENTIFIER);
    Identifier indexVariable = Identifier(this->tokens[this->pointer]);
    return For(startExpression, endExpression, stepExpression, indexVariable, statements);
}

If Parser::if_(void)
{
    // <if> ::= [IF] <expression> [THEN] (<statement>)* [ENDIF]
    Node condition = expression();
    advance();
    match(Tokens::THEN);
    advance();
    std::vector<Node> statements;
    while (!(compare(Tokens::ENDIF) || compare(Tokens::ELSE)) && this->pointer != -1)
    {
        statements.push_back(statement());
        advance();
    }

    if (this->pointer == -1) { throw Missed_Terminating_Token(Keywords::ENDIF); }
    
    if (compare(Tokens::ELSE)) 
    {
        advance(); 
        Else elseNode = else_();
        return If(condition, statements, elseNode);
    }
    else
    {
        return If(condition, statements);
    }
}

Else Parser::else_(void)
{
    std::vector<Node> statements;
    while (!compare(Tokens::ENDIF) && this->pointer != -1)
    {
        Node statementNode = statement();
        statements.push_back(statementNode);
        advance();
    }
    
    if (this->pointer == -1) { throw Missed_Terminating_Token(Keywords::ENDIF); }
    return Else(statements);
}

While Parser::while_(void)
{
    // <while> ::= [WHILE] <expression> (<statement>)* [ENDWHILE]
    Node condition = expression();
    advance();

    std::vector<Node> statements;
    while (!compare(Tokens::ENDWHILE) && this->pointer != -1)
    {
        statements.push_back(statement());
        advance();
    }

    if (this->pointer == -1)
    {
        throw Missed_Terminating_Token(Keywords::ENDWHILE);
    }
    return While(condition, statements);
}

Return Parser::return_(void)
{
    // <return> ::= [RETURN] <expression>
    Node expressionNode = expression();
    return Return(expressionNode); 
}

Function Parser::function(void)
{
    // <function> ::= [FUNCTION] [IDENTIFIER] ( <function-parameter> [RETURNS] <primitive-type> <statement> [ENDFUNCTION]
    match(Tokens::IDENTIFIER);
    Identifier functionName = Identifier(this->tokens[this->pointer]);
    advance();
    match(Tokens::LBRACKET);
    advance();

    unsigned int argumentCount = 0;
    Function_Arguments arguments;
    while (!compare(Tokens::RBRACKET) && this->pointer != -1)
    {
        if (argumentCount > 0)  // one comma to separate each function argument
        {
            match(Tokens::COMMA);
            advance();
        }

        match(Tokens::IDENTIFIER);
        Identifier identifier = Identifier(this->tokens[this->pointer]);
        advance();
        match(Tokens::COLON);
        advance();
        Primitive type = primitive_type();
        advance();
        
        arguments.add_argument(identifier, type);
        argumentCount++;
    }

    if (this->pointer == -1) { throw Missed_Terminating_Token(Keywords::LBRACKET); }
    advance();
    match(Tokens::RETURNS);
    advance();

    Primitive returnType = primitive_type();
    advance();

    std::vector<Node> statements;
    while (!compare(Tokens::ENDFUNCTION) && this->pointer != -1)
    {
        statements.push_back(statement());
        advance();
    }

    if (this->pointer == -1) { throw Missed_Terminating_Token(Keywords::ENDFUNCTION); }
    return Function(functionName, returnType, arguments, statements);
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

Node Parser::expression(void)
{ 
    //<expression> ::= <equality>
    return logical_comparison();
}

Node Parser::logical_comparison(void) // most expression nodes are in this structure
{
    Node expression = equality(); // form left side of expression

    while (peek(Tokens::AND) || peek(Tokens::OR)) // check if the respective operator is next
    {
        Token operation = this->tokens[this->pointer]; // collect operator
        advance();
        Node rightExpression = equality(); // form lower precedence expression 
        expression = Binary_Expression(expression, operation, rightExpression); // form node
    }
    return expression;
}

Node Parser::equality(void)
{
    Node expression = numerical_comparison();

    while (peek(Tokens::EQUAL) || peek(Tokens::NOT_EQUAL))
    {
        Token operation = this->tokens[this->pointer];
        advance();
        Node rightExpression = numerical_comparison();
        expression = Binary_Expression(expression, operation, rightExpression);
    }

    return expression;
}

Node Parser::numerical_comparison(void)
{
    Node expression = term();

    while (peek(Tokens::GREATER) || peek(Tokens::GREATER_EQUAL) || peek(Tokens::LESSER) || peek(Tokens::LESSER_EQUAL))
    {
        Token operation = this->tokens[this->pointer];
        advance();
        Node rightExpression = term();
        expression = Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node Parser::term(void)
{
    Node expression = factor();

    while (peek(Tokens::ADDITION) || peek(Tokens::SUBTRACTION))
    {
        Token operation = this->tokens[this->pointer];
        advance();
        Node rightExpression = factor();
        expression = Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node Parser::factor(void)
{
    Node expression = unary();

    while (peek(Tokens::MULTIPLICATION) || peek(Tokens::DIVISION))
    {
        Token operation = this->tokens[this->pointer];
        advance();
        Node rightExpression = unary();
        expression = Binary_Expression(expression, operation, rightExpression);
    }
    return expression;
}

Node Parser::unary(void)
{
    if (compare(Tokens::SUBTRACTION) || compare(Tokens::NOT))
    {
        Token unaryOperator = this->tokens[this->pointer];
        advance();
        return Unary_Expression(unary(), unaryOperator); // create and return unary expression
    }
    else
    {
        return primary();
    }
}

Node Parser::primary(void)
{
    if (compare(Tokens::IDENTIFIER))
    {
        Identifier identifier = Identifier(this->tokens[this->pointer]);
        std::cout << this->tokens[this->pointer].type << '\n';
        if (peek(Tokens::LBRACKET)) // try form function call node
        {
            advance();
            Call_Arguments arguments = function_call_arguments();
            return Function_Call(identifier, arguments);
        }

        if (peek(Tokens::ASSIGNMENT)) // try form variable assignment node
        {
            advance();
            Node assignmentExpression = expression();
            return Variable_Assignment(identifier, assignmentExpression);
        }
        else if (peek(Tokens::END_OF_LINE))
        {
            return identifier; // return a regular literal
        }
        
        throw Generic_Error();
    }
    else if (compare(Tokens::LBRACKET)) // attempt to form bracketed expression
    {
        advance();
        Node expressionNode = expression();
        advance();
        match(Tokens::RBRACKET);
        return expressionNode;
    }
    else
    {
        return primitive_literal();
    }
}

Call_Arguments Parser::function_call_arguments(void)
{
    // <function-call-parameter> ::= <expression> , <function-call-parameter> | )
    Call_Arguments arguments;
    arguments.add_argument(expression());
    advance();
    while (!(compare(Tokens::COMMA) || compare(Tokens::RBRACKET)) && this->pointer != -1)
    {
        arguments.add_argument(expression());
        advance();
    }
    return arguments;
}

// END EXPRESSION //