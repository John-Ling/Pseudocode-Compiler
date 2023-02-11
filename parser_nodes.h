#ifndef Parser_Nodes_H
#define Parser_Nodes_H

#include <iostream>

typedef struct Primitive_Literal_T
{
    std::string value;
} Primitive_Literal;


typedef struct Add_Node_T
{
    std::string operand1;
    std::string operand2;
} Add_Node;

typedef struct Subtract_Node_T
{
    std::string operand1;
    std::string operand2;
} Subtract_Node;

typedef struct Multiplication_Node_T
{
    std::string operand1;
    std::string operand2;
} Multiplication_Node;

typedef struct Division_Node_T
{
    std::string operand1;
    std::string operand2;
} Division_Node;

#endif