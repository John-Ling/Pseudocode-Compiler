#ifndef Ast_Nodes_H
#define Ast_Nodes_H

// definitions of all abstract syntax tree nodes

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
        std::string value;
        std::string type;
    public:
        Literal(void);
        Literal(Token token);
        std::string get_value(void);
        std::string get_type(void);
};

class Identifier: public Node
{
    private:
        std::string variableName;
    public:
        Identifier(void);
        Identifier(Token token);
        std::string get_variable_name(void);
};

class Primitive: public Node
{
    private:
        std::string type; 
    public:
        Primitive(void);
        Primitive(Token token);
        std::string get_type(void);
};

class Variable_Declaration: public Node
{
    private:
        Identifier identifier;
        Primitive type; // primitive node
    public:
        Variable_Declaration(Identifier identifier, Primitive type);
        Identifier get_identifier(void);
        Primitive get_type(void);
};

class Variable_Assignment: public Node
{
    private:
        Identifier identifier;
        Node expression;
    public:
        Variable_Assignment(Identifier identifier, Node expression);
        Identifier get_identifier(void);
        Node get_expression(void);
};

class Output: public Node
{
    private:
        std::vector<Node> expressions;
    public:
        Output(std::vector<Node> expressions);
        std::vector<Node> get_expressions(void);
};

class Input: public Node
{
    private:
        Identifier identifier;
    public:
        Input(Identifier identifier);
        Identifier get_identifier(void);
};

class Function_Arguments: public Node
{
    private:
        std::unordered_map<std::string, Primitive> arguments;
    public:
        Function_Arguments(void);
        void add_argument(Identifier identifier, Primitive type);
        std::unordered_map<std::string, Primitive> get_arguments(void);
};

class Function: public Node
{
    private:
        Identifier functionName;
        Function_Arguments arguments; // function_parameter node
        Primitive returnType;
        std::vector<Node> statements;
    public:
        Function(Identifier functionName, Primitive returnType, Function_Arguments arguments, std::vector<Node> statements);
        Identifier get_function_name(void);
        Function_Arguments get_arguments(void);
        Primitive get_return_type(void);
        std::vector<Node> get_statements(void);
};

class Call_Arguments: public Node
{
    private:
        std::vector<Node> arguments;
    public:
        Call_Arguments(void);
        void add_argument(Node expression);
        std::vector<Node> get_arguments(void);
};

class Function_Call: public Node
{
    private:
        Identifier functionName;
        Call_Arguments arguments;
    public:
        Function_Call(Identifier functionName, Call_Arguments arguments);
        std::vector<Node> get_arguments(void);
};

class While: public Node
{
    private:
        Node condition;
        std::vector<Node> statements;
    public:
        While(Node condition, std::vector<Node> statements);
        Node get_condition(void);
        std::vector<Node> get_statements(void);
};

class For: public Node
{
    private:
        Node start;
        Node end;
        Node step;
        Identifier indexVariable; // identifier node
        std::vector<Node> statements;
    public:
        For(Node start, Node end, Identifier indexVariable, std::vector<Node> statements);
        For(Node start, Node end, Node step, Identifier indexVariable, std::vector<Node> statements);
        Node get_start(void);
        Node get_end(void);
        Node get_step(void);
        Identifier get_index_variable(void);
        std::vector<Node> get_statements(void);
};


class Else: public Node
{
    private:
        std::vector<Node> statements;
    public:
        Else(void);
        Else(std::vector<Node> statements);
        std::vector<Node> get_statements(void);
};

class If: public Node
{
    private:
        Node condition;
        Else elseNode; // Else node
        std::vector<Node> statements;
        bool elseNodePresent;
    public:
        If(void);
        If(Node condition, std::vector<Node> statements);
        If(Node condition, std::vector<Node> statements, Else elseNode);
        Node get_condition(void);
        std::vector<Node> get_statements();
        bool is_else_node_present(void);
        Else get_else_node(void);
};


class Return: public Node
{
    private:
        Node expression;
    public:
        Return(Node expression);
        Node get_expression(void);
};

class Binary_Expression: public Node
{
    private:
        Token operation;
        Node left;
        Node right;
    public:
        Binary_Expression(Node left, Token operation, Node right);
        Token get_operator(void);
        Node get_left_expression(void);
        Node get_right_expression(void);
};

class Unary_Expression: public Node
{
    private:
        Node expression;
        Token operation;
    public:
        Unary_Expression(Node expression, Token operation);
        Token get_operator(void);
        Node get_expression(void);
};

#endif