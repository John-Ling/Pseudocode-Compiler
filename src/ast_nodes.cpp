#include "ast_nodes.h"

void Node::set_name(std::string value)
{
    this->NAME = value;
};

Literal::Literal(Token value)
{
    set_name(AST_Node_Names::LITERAL);
    this->value = value;
}

Identifier::Identifier(std::string identifierName)
{
    set_name(AST_Node_Names::IDENTIFIER);
    this->identifierName = identifierName;
}

Primitive::Primitive(Token value)
{
    set_name(AST_Node_Names::PRIMITIVE);
    this->value = value;
}

Variable_Declaration::Variable_Declaration(Token identifier, Node* type)
{
    set_name(AST_Node_Names::DECLARATION);
    this->identifier = identifier;
    this->type = type;
}

Variable_Assignment::Variable_Assignment(Token identifier, Node* expression)
{
    set_name(AST_Node_Names::ASSIGNMENT);
    this->identifier = identifier;
    this->expression = expression;
}

Output::Output(std::vector<Node*> expressions)
{
    set_name(AST_Node_Names::OUTPUT);
    this->expressions = expressions;
}

Input::Input(Token identifier)
{
    set_name(AST_Node_Names::INPUT);
    this->identifier = identifier;
}

Function::Function(Node* functionName, Token returnType, Node* parameters, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::FUNCTION);
    this->functionName = functionName;
    this->returnType = returnType;
    this->parameters = parameters;
    this->statements = statements;
}

Return::Return(Node* expression)
{
    set_name(AST_Node_Names::RETURN);
    this->expression = expression;
}

While::While(Node* condition, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::WHILE);
    this->condition = condition;
    this->statements = statements;
}

For::For(int start, int end, int step, Node* indexVariable, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::FOR);
    this->start = start;
    this->end = end;
    this->step = step;
    this->indexVariable = indexVariable;
    this->statements = statements;
}

If::If(Node* condition, std::vector<Node*> statements, Node* elseNode)
{
    set_name(AST_Node_Names::IF);
    this->condition = condition;
    this->statements = statements;
    this->elseNode = elseNode;
}

Else::Else(std::vector<Node*> statements)
{
    set_name(AST_Node_Names::ELSE);
    this->statements = statements;
}

Binary_Expression::Binary_Expression(Node* left, Token operation, Node* right)
{
    set_name(AST_Node_Names::BINARY_EXPRESSION);
    this->left = left;
    this->operation = operation;
    this->right = right;
}

Unary_Expression::Unary_Expression(Node* expression, Token operation)
{
    set_name(AST_Node_Names::UNARY_EXPRESSION);
    this->expression = expression;
    this->operation = operation;
}

