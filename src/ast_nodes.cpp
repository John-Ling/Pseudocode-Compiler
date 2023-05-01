#include "ast_nodes.h"

void Node::set_name(std::string value)
{
    this->NAME = value;
}

std::string Node::get_node_name(void)
{
    return this->NAME;
}

Literal::Literal(void) 
{
    set_name(AST_Node_Names::LITERAL);
}

Literal::Literal(Token token)
{
    set_name(AST_Node_Names::LITERAL);
    this->value = token.value;
    this->type = token.type;
}

std::string Literal::get_type(void)
{
    return this->type;
}

std::string Literal::get_value(void)
{
    return this->value;
}

Identifier::Identifier(void) 
{
    set_name(AST_Node_Names::IDENTIFIER);
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

Primitive::Primitive(void)
{
    set_name(AST_Node_Names::PRIMITIVE);
}

Primitive::Primitive(Token token)
{
    set_name(AST_Node_Names::PRIMITIVE);
    this->type = token.type;
}

std::string Primitive::get_type(void)
{
    return this->type;
}


Variable_Declaration::Variable_Declaration(Identifier identifier, Primitive type)
{
    set_name(AST_Node_Names::DECLARATION);
    this->identifier = identifier;
    this->type = type;
}

Identifier Variable_Declaration::get_identifier(void)
{
    return this->identifier;
}

Primitive Variable_Declaration::get_type(void)
{
    return this->type;
}

Variable_Assignment::Variable_Assignment(Identifier identifier, Node expression)
{
    set_name(AST_Node_Names::ASSIGNMENT);
    this->identifier = identifier;
    this->expression = expression;
}

Identifier Variable_Assignment::get_identifier(void)
{
    return this->identifier;
}

Node Variable_Assignment::get_expression(void)
{
    return this->expression;
}

Output::Output(std::vector<Node> expressions)
{
    set_name(AST_Node_Names::OUTPUT);
    this->expressions = expressions;
}

std::vector<Node> Output::get_expressions(void)
{
    return this->expressions;
}

Input::Input(Identifier identifier)
{
    set_name(AST_Node_Names::INPUT);
    this->identifier = identifier;
}

Identifier Input::get_identifier(void)
{
    return this->identifier;
}

Function::Function(Identifier functionName, Primitive returnType, Function_Arguments arguments, std::vector<Node> statements)
{
    set_name(AST_Node_Names::FUNCTION);
    this->functionName = functionName;
    this->returnType = returnType;
    this->arguments = arguments;
    this->statements = statements;
}

Identifier Function::get_function_name(void)
{
    return this->functionName;
}

Function_Arguments Function::get_arguments(void)
{
    return this->arguments;
}

Primitive Function::get_return_type(void)
{
    return this->returnType;
}

std::vector<Node> Function::get_statements(void)
{
    return this->statements;
}

Function_Arguments::Function_Arguments(void)
{
    set_name(AST_Node_Names::FUNCTION_ARGUMENTS);
}

std::unordered_map<std::string, Primitive> Function_Arguments::get_arguments(void)
{
    return this->arguments;
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

Function_Call::Function_Call(Identifier functionName, Call_Arguments arguments)
{
    this->functionName = functionName;
    this->arguments = arguments;
}

Call_Arguments::Call_Arguments(void)
{
    set_name(AST_Node_Names::CALL_ARGUMENTS);
}

void Call_Arguments::add_argument(Node expression)
{
    this->arguments.push_back(expression); 
}

std::vector<Node> Call_Arguments::get_arguments(void)
{
    return this->arguments;
}

Return::Return(Node expression)
{
    set_name(AST_Node_Names::RETURN);
    this->expression = expression;
}

Node Return::get_expression(void)
{
    return this->expression;
}

While::While(Node condition, std::vector<Node> statements)
{
    set_name(AST_Node_Names::WHILE);
    this->condition = condition;
    this->statements = statements;
}

Node While::get_condition(void)
{
    return this->condition;
}

std::vector<Node> While::get_statements(void)
{
    return this->statements;
}

For::For(Node start, Node end, Identifier indexVariable, std::vector<Node> statements)
{
    set_name(AST_Node_Names::FOR);
    this->start = start;
    this->end = end;
    this->indexVariable = indexVariable;
    this->statements = statements;
}

For::For(Node start, Node end, Node step, Identifier indexVariable, std::vector<Node> statements)
{
    set_name(AST_Node_Names::FOR);
    this->start = start;
    this->end = end;
    this->step = step;
    this->indexVariable = indexVariable;
    this->statements = statements;
}

Node For::get_start(void)
{
    return this->start;
}

Node For::get_end(void)
{
    return this->end;
}

Node For::get_step(void)
{
    return this->step;
}

Identifier For::get_index_variable(void)
{
    return this->indexVariable;
}

std::vector<Node> For::get_statements(void)
{
    return this->statements;
}

If::If(void)
{
    set_name(AST_Node_Names::IF);
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

Node If::get_condition(void)
{
    return this->condition;
}

std::vector<Node> If::get_statements(void)
{
    return this->statements;
}

bool If::is_else_node_present(void)
{
    return this->elseNodePresent;
}

Else If::get_else_node(void)
{
    return this->elseNode;
}

Else::Else(void)
{
    set_name(AST_Node_Names::ELSE);
}

Else::Else(std::vector<Node> statements)
{
    set_name(AST_Node_Names::ELSE);
    this->statements = statements;
}

std::vector<Node> Else::get_statements(void)
{
    return this->statements;
}

Binary_Expression::Binary_Expression(Node left, Token operation, Node right)
{
    set_name(AST_Node_Names::BINARY_EXPRESSION);
    this->left = left;
    this->operation = operation;
    this->right = right;
}

Node Binary_Expression::get_left_expression(void)
{
    return this->left;
}

Token Binary_Expression::get_operator(void)
{
    return this->operation;
}

Node Binary_Expression::get_right_expression(void)
{
    return this->right;
}

Unary_Expression::Unary_Expression(Node expression, Token operation)
{
    set_name(AST_Node_Names::UNARY_EXPRESSION);
    this->expression = expression;
    this->operation = operation;
}

Node Unary_Expression::get_expression(void)
{
    return this->expression;
}

Token Unary_Expression::get_operator(void)
{
    return this->operation;
}