#ifndef Parser_Nodes_H
#define Parser_Nodes_H

#include <iostream>
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

typedef struct Primitive_Literal_T
{
    Token value;
    Primitive_Literal_T(Token value)
    {
        this->value = value;
    }
} Primitive_Literal;

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