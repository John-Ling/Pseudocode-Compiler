#include "ast_nodes.h"

void Node::set_name(std::string value)
{
    this->NAME = value;
}

std::string Node::get_node_name(void)
{
    return this->NAME;
}

Literal::Literal(Token token)
{
    set_name(AST_Node_Names::LITERAL);
    this->value = token.value;
    this->type = token.type;
}

Identifier::Identifier(Token token)
{
    set_name(AST_Node_Names::IDENTIFIER);
    this->variableName = token.value;
}

std::string Identifier::get_variable_name(void) 
{ 
    return this->variableName; 
}

Primitive::Primitive(Token token)
{
    set_name(AST_Node_Names::PRIMITIVE);
    this->type = token.type;
}

Variable_Declaration::Variable_Declaration(Identifier identifier, Primitive type)
{
    set_name(AST_Node_Names::DECLARATION);
    this->identifier = identifier;
    this->type = type;
}

Variable_Assignment::Variable_Assignment(Identifier identifier, Node expression)
{
    set_name(AST_Node_Names::ASSIGNMENT);
    this->identifier = identifier;
    this->expression = expression;
}

Output::Output(std::vector<Node> expressions)
{
    set_name(AST_Node_Names::OUTPUT);
    this->expressions = expressions;
}

Input::Input(Identifier identifier)
{
    set_name(AST_Node_Names::INPUT);
    this->identifier = identifier;
}

Function::Function(Identifier functionName, Primitive returnType, Function_Arguments arguments, std::vector<Node> statements)
{
    set_name(AST_Node_Names::FUNCTION);
    this->functionName = functionName;
    this->returnType = returnType;
    this->arguments = arguments;
    this->statements = statements;
}

Function_Arguments::Function_Arguments(void)
{
    set_name(AST_Node_Names::FUNCTION_ARGUMENTS);
}

Function_Call::Function_Call(Identifier functionName, Call_Arguments arguments)
{
    this->functionName = functionName;
    this->arguments = arguments;
}

void Call_Arguments::add_argument(Node expression)
{
    this->arguments.push_back(expression); 
}

void Function_Arguments::add_argument(Identifier identifier, Primitive type)
{
    std::string identifierName = identifier.get_variable_name();
    if (this->arguments.count(identifierName)) 
    {
        this->arguments[identifierName] = type; 
    }
    return;
}

Return::Return(Node expression)
{
    set_name(AST_Node_Names::RETURN);
    this->expression = expression;
}

While::While(Node condition, std::vector<Node> statements)
{
    set_name(AST_Node_Names::WHILE);
    this->condition = condition;
    this->statements = statements;
}

For::For(Node start, Node end, Node* step, Identifier indexVariable, std::vector<Node> statements)
{
    set_name(AST_Node_Names::FOR);
    this->start = start;
    this->end = end;
    this->step = step;
    this->indexVariable = indexVariable;
    this->statements = statements;
}

If::If(Node condition, std::vector<Node> statements)
{
    set_name(AST_Node_Names::IF);
    this->condition = condition;
    this->statements = statements;
    this->elseNode = elseNode;
    this->elseNodePresent = false;
}

If::If(Node condition, std::vector<Node> statements, Else elseNode)
{
    set_name(AST_Node_Names::IF);
    this->condition = condition;
    this->statements = statements;
    this->elseNode = elseNode;
    this->elseNodePresent = true;
}

Else::Else(std::vector<Node> statements)
{
    set_name(AST_Node_Names::ELSE);
    this->statements = statements;
}

Binary_Expression::Binary_Expression(Node left, Token operation, Node right)
{
    set_name(AST_Node_Names::BINARY_EXPRESSION);
    this->left = left;
    this->operation = operation;
    this->right = right;
}

Unary_Expression::Unary_Expression(Node expression, Token operation)
{
    set_name(AST_Node_Names::UNARY_EXPRESSION);
    this->expression = expression;
    this->operation = operation;
}