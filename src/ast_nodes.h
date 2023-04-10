#ifndef Ast_Nodes_H
#define Ast_Nodes_H

#include <iostream>
#include <vector>
#include "token.h"
#include "constants.h"

typedef struct Node_T{} Node; // abstract-like class to represent all nodes in one keyword




typedef struct Variable_Declaration_T: Node
{
    const std::string NAME = "variable_declaration";
    Token identifier;
    Token type;
    Variable_Declaration_T(Token identifier, Token type)
    {
        this->identifier = identifier;
        this->type = type;
    }
} Variable_Declaration;

typedef struct Binary_Expression_T: Node
{
    const std::string NAME = "binary_expression";
    Node left;
    Node right;
    Token expressionOperator;
    Binary_Expression_T(Node left, Node right, Token expressionOperator) 
    {
        this->left = left;
        this->right = right;
        this->expressionOperator = expressionOperator;
    }
} Binary_Expression;

typedef struct Unary_Expression_T: Node
{
    const std::string NAME = "unary_expression";
    Node expression;
    Token unaryOperator;
    Unary_Expression_T(Node expression, Token unaryOperator)
    {
        this->expression = expression;
        this->unaryOperator = unaryOperator;
    }
} Unary_Expression;

typedef struct Literal_T: Node
{
    const std::string NAME = "literal";
    Token value;
    Literal_T(Token value)
    {
        this->value = value;
    }
} Literal;

#endif