#ifndef Ast_Nodes_H
#define Ast_Nodes_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "token.h"
#include "constants.h"
#include "exceptions.h"

class Node
{
    private:
        std::string NAME;
    public:
        std::string get_node_name(void);
        void set_name(std::string value);
};

class Literal: public Node
{
    private:
        Token value;
    public:
        Literal(Token value);
        Token get_value(void);
};

class Identifier: public Node
{
    private:
        std::string identifierName;
    public:
        Identifier(std::string identifierName);
        std::string get_identifier_name(void);
};

class Primitive: public Node
{
    private:
        Token value;
    public:
        Primitive(Token value);
        Token get_value(void);
};

class Variable_Declaration: public Node
{
    private:
        Token identifier;
        Node* type; // primitive node
    public:
        Variable_Declaration(Token identifier, Node* type);
        Token get_identifier(void);
        Primitive get_type(void);
};

class Variable_Assignment: public Node
{
    private:
        Token identifier;
        Node* expression;
    public:
        Variable_Assignment(Token identifier, Node* expression);
        Token get_identifier(void);
        Node* get_expression(void);
};

class Output: public Node
{
    private:
        std::vector<Node*> expressions;
    public:
        Output(std::vector<Node*> expressions);
        std::vector<Node*> get_expressions(void);
};

class Input: public Node
{
    private:
        Token identifier;
    public:
        Input(Token identifier);
        Token get_identifier(void);
};

class Function: public Node
{
    private:
        Node* functionName; // literal node
        Node* parameters; // function_parameter node
        Token returnType;
        std::vector<Node*> statements;
    public:
        Function(Node* functionName, Token returnType, Node* parameters, std::vector<Node*> statements);
        Token get_function_name(void);
        Token get_parameters(void);
        Token get_return_type(void);
        std::vector<Node*> get_statements(void);
};

class Function_Parameter: public Node
{
    private:
        std::unordered_map<Token, Token> arguments;
    public:
        Function_Parameter(void);
        void add_argument(Token key, Token value);
        std::unordered_map<Token, Token> get_arguments(void);
};

class While: public Node
{
    private:
        Node* condition;
        std::vector<Node*> statements;
    public:
        While(Node* condition, std::vector<Node*> statements);
        Node* get_condition(void);
        std::vector<Node*> get_statements(void);
};

class For: public Node
{
    private:
        int start;
        int end;
        int step;
        Node* indexVariable; // literal node
        std::vector<Node*> statements;
    public:
        For(int start, int end, int step, Node* indexVariable, std::vector<Node*> statements);
        int get_start(void);
        int get_end(void);
        int get_step(void);
        Node* get_index_variable(void);
        std::vector<Node*> get_statements(void);
};

class If: public Node
{
    private:
        Node* condition;
        Node* elseNode; // Else node
        std::vector<Node*> statements;
    public:
        If(Node* condition, std::vector<Node*> statements, Node* elseNode);
        Node* get_condition(void);
        Node* get_else_node(void);
};

class Else: public Node
{
    private:
        std::vector<Node*> statements;
    public:
        Else(std::vector<Node*> statements);
        std::vector<Node*> get_statements(void);
};

class Return: public Node
{
    private:
        Node* expression;
    public:
        Return(Node* expression);
        Node* get_expression(void);
};

class Function_Call: public Node
{
    private:
        Token functionName;
        std::vector<Node*> parameters;
    public:
        Function_Call(Token functionName, std::vector<Node*> parameters);
        Token get_function_name(void);
};

class Binary_Expression: public Node
{
    private:
        Token operation;
        Node* left;
        Node* right;
    public:
        Binary_Expression(Node* left, Token operation, Node* right);
        Token get_operator(void);
        Node* get_left_expression(void);
        Node* get_right_expression(void);
};

class Unary_Expression: public Node
{
    private:
        Node* expression;
        Token operation;
    public:
        Unary_Expression(Node* expression, Token operation);
        Token get_operator(void);
        Node* get_expression(void);
};

#endif