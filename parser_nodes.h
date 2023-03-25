#ifndef Parser_Nodes_H
#define Parser_Nodes_H

#include <iostream>
#include <vector>
#include "token.h"

typedef struct Variable_Assignment_T
{
    Token expression;
    Token variable;
    Variable_Assignment_T(Token variable, Token expression)
    {
        this->variable = variable;
        this->expression = expression;
    }
} Variable_Assignment;

typedef struct Variable_Declaration_T
{
    Token identifier;
    Token datatype;
    Variable_Declaration_T(Token identifier, Token datatype)
    {
        this->identifier = identifier;
        this->datatype = datatype;
    }
} Variable_Declaration;

typedef struct Output_T
{
    Token expression;
    Output_T(Token expression)
    {
        this->expression = expression;
    }
} Output;

typedef struct Input_T
{
    Token identifier;
    Token expression;
    Input_T(Token identifier, Token expression)
    {
        this->identifier = identifier;
        this->expression = expression;
    }
} Input;

typedef struct Function_Argument_T
{
    Token expression;
    Token dataType;
    Function_Argument_T(Token expression, Token dataType)
    {
        this->expression = expression;
        this->dataType = dataType;
    }
} Function_Argument;

typedef struct Function_T
{
    Token functionName;
    Token returnType;
    std::vector<Function_Argument> functionArguments;
    std::vector<Token> statements; // stores all code within the function 
    Function_T(Token functionName, Token returnType, std::vector<Function_Argument> functionArguments, std::vector<Token> statements)
    {
        this->functionName = functionName;
        this->returnType = returnType;
        this->functionArguments = functionArguments;
        this->statements = statements;
    }
} Function;

typedef struct While_T
{
    Token expression;
    std::vector<Token> statements;
    While_T(Token expression, std::vector<Token> statements)   
    {
        this->expression = expression;
        this->statements = statements;
    }
} While;

typedef struct Primitive_Literal_T
{
    Token value;
    Primitive_Literal_T(Token value)
    {
        this->value = value;
    }
} Primitive_Literal;

typedef struct Expression_T
{
    Token expression;
}

typedef struct Binary_Expression_T
{
    Token expression1;
    Token expression2;
    Token expressionOperator;
    Binary_Expression_T(Token expression1, Token expressionOperator, Token expression2)
    {
        this->expression1 = expression1;
        this->expressionOperator = expressionOperator;
        this->expression2 = expression2;
    }
} Binary_Expression;

typedef struct Unary_Expression_T
{
    Token expression;
    Token unaryOperator;
    Unary_Expression_T(Token expression, Token unaryOperator)
    {
        this->expression = expression;
        this->unaryOperator = unaryOperator;
    }
} Unary_Expression;

#endif