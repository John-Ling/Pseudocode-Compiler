#ifndef Ast_Nodes_Hpp
#define Ast_Nodes_Hpp

// definitions of all abstract syntax tree nodes

#include <vector>
#include <unordered_map>
#include "token.hpp"
#include "constants.hpp"
#include "exceptions.hpp"

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
        bool declared;
    public:
        Identifier(void);
        Identifier(Token token);
        std::string get_variable_name(void);
        bool get_declared(void);
        void set_declared(bool value);
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

class Array: public Node
{
    private: // array can have up to two lower and upper bounds if it a 2D array
        Node* type;
        std::vector<Literal*> lowerBounds; // Refers to the index of the first array can be 1 or 0
        std::vector<Literal*> upperBounds; // Refers to the size of the array cannot be 0
    public:
        Array(void);
        Array(Node* token, std::vector<Literal*> lowerBounds, std::vector<Literal*> upperBounds);
        Primitive* get_type(void);
        std::vector<Literal*> get_lower_bounds(void);
        std::vector<Literal*> get_upper_bounds(void);
};

class Array_Assignment: public Node
{
    private:
        Node* expression;
        std::vector<Node*> indexExpressions; // There can be potentially two index expressions if it's a 2D array 
        Node* identifier; // array name
    public:
        Array_Assignment(Node* identifier, Node* expression, std::vector<Node*> indexExpressions);
        Node* get_expression(void);
        Identifier* get_identifier(void);
        std::vector<Node*> get_index_expressions(void);
};

class Array_Expression: public Node
{
    private:
        std::vector<Node*> indexExpressions;
    public:
        Array_Expression(std::vector<Node*> indexExpressions);
        std::vector<Node*> get_index_expressions(void);
};

class Variable_Declaration: public Node
{
    private:
        Node* identifier;
        Node* type; // primitive node
    public:
        Variable_Declaration(Node* identifier, Node* type);
        Identifier* get_identifier(void);
        Primitive* get_type(void);
};

class Variable_Assignment: public Node
{
    private:
        Node* identifier;
        Node* expression;
    public:
        Variable_Assignment(Node* identifier, Node* expression);
        Identifier* get_identifier(void);
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
        Node* identifier;
    public:
        Input(Node* identifier);
        Identifier* get_identifier(void);
};

class Function_Arguments: public Node
{
    private:
        std::vector<std::string> argumentNames;
        std::vector<Node*> argumentTypes;
    public:
        Function_Arguments(void);
        void add_argument(Node* identifier, Node* type);
        std::vector<std::string> get_argument_names(void);
        std::vector<Node*> get_argument_types(void);
};

class Function: public Node
{
    private:
        Node* functionName; // identifier
        Node* arguments; // function_parameter node
        Node* returnType; // primitive
        std::vector<Node*> statements;
    public:
        Function(Node* functionName, Node* returnType, Node* arguments, std::vector<Node*> statements);
        Identifier* get_function_name(void);
        Function_Arguments* get_arguments(void);
        Primitive* get_return_type(void);
        std::vector<Node*> get_statements(void);
};

class Call_Arguments: public Node
{
    private:
        std::vector<Node*> arguments;
    public:
        Call_Arguments(void);
        void add_argument(Node* expression);
        std::vector<Node*> get_arguments(void);
};

class Function_Call: public Node
{
    private:
        Node* functionName;
        Node* arguments;
    public:
        Function_Call(Node* functionName, Node* arguments);
        Identifier* get_function_name(void);
        Call_Arguments* get_arguments(void);
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
        Node* start;
        Node* end;
        Node* step;
        Node* indexVariable; // identifier node
        std::vector<Node*> statements;
        bool stepPresent;
    public:
        For(Node* start, Node* end, Node* indexVariable, std::vector<Node*> statements);
        For(Node* start, Node* end, Node* step, Node* indexVariable, std::vector<Node*> statements);
        Node* get_start(void);
        Node* get_end(void);
        Node* get_step(void);
        Identifier* get_index_variable(void);
        std::vector<Node*> get_statements(void);
        bool is_step_present(void);
};

class Else: public Node
{
    private:
        std::vector<Node*> statements;
    public:
        Else(void);
        Else(std::vector<Node*> statements);
        std::vector<Node*> get_statements(void);
};

class If: public Node
{
    private:
        Node* condition;
        Node* elseNode; // Else node
        std::vector<Node*> statements;
        bool elseNodePresent;
    public:
        If(void);
        If(Node* condition, std::vector<Node*> statements);
        If(Node* condition, std::vector<Node*> statements, Node* elseNode);
        Node* get_condition(void);
        std::vector<Node*> get_statements();
        bool is_else_node_present(void);
        Else* get_else_node(void);
};

class Return: public Node
{
    private:
        Node* expression;
    public:
        Return(Node* expression);
        Node* get_expression(void);
};

class Bracketed_Expression: public Node
{
    private:
        Node* expression;
    public:
        Bracketed_Expression(Node* expression);
        Node* get_expression(void);
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