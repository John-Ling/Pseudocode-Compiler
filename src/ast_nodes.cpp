#include "ast_nodes.hpp"

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
    this->declared = false; // declared will be set to true later on in semantic analysis
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

bool Identifier::get_declared(void)
{
    return this->declared;
}

void Identifier::set_declared(bool value)
{
    this->declared = value;
    return;
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

Array::Array(Node* identifier, Node* type, std::vector<Literal*> lowerBounds, std::vector<Literal*> upperBounds)
{
    set_name(AST_Node_Names::ARRAY_DECLARATION);
    this->type = type;
    this->identifier = identifier;

    unsigned int count = lowerBounds.size();

    if (lowerBounds.size() > 2 || lowerBounds.size() == 0)
    {
        throw Generic_Error("Too many or 0 lower bounds have been provided for array.");
    }

    if (upperBounds.size() > 2 || upperBounds.size() == 0)
    {
        throw Generic_Error("Too many or 0 upper bounds have been provided for array.");
    }

    for (unsigned int i = 0; i < count; i++) // validation and assignment code
    {
        if (upperBounds[i]->get_value() == "0")
        {
            throw Generic_Error("Upper bound for array cannot be 0. ");
        }
        if (lowerBounds[i]->get_value() != "0" && lowerBounds[i]->get_value() != "1")
        {
            throw Generic_Error("Lower bound for array must be 0 or 1.");
        }

        this->lowerBounds.push_back(lowerBounds[i]);
        this->upperBounds.push_back(upperBounds[i]);
    }
}

Identifier* Array::get_identifier(void)
{
    return static_cast<Identifier*>(this->identifier);
}

Primitive* Array::get_type(void)
{
    return static_cast<Primitive*>(this->type);
}

std::vector<Literal*> Array::get_lower_bounds(void)
{
    return this->lowerBounds;
}

std::vector<Literal*> Array::get_upper_bounds(void)
{
    return this->upperBounds;
}

Array_Assignment::Array_Assignment(Node* identifier, Node* expression, std::vector<Node*> indexExpressions)
{
    set_name(AST_Node_Names::ARRAY_ASSIGNMENT);
    this->identifier = identifier;
    this->expression = expression;
    this->indexExpressions = indexExpressions;
    }

Identifier* Array_Assignment::get_identifier(void)
{
    return static_cast<Identifier*>(this->identifier);
}

Node* Array_Assignment::get_expression(void)
{
    return this->expression;
}

std::vector<Node*> Array_Assignment::get_index_expressions(void)
{
    return this->indexExpressions;
}

Array_Expression::Array_Expression(Node* identifier, std::vector<Node*> indexExpressions)
{
    set_name(AST_Node_Names::ARRAY_EXPRESSION);
    this->identifier = identifier;
    this->indexExpressions = indexExpressions;
}

std::vector<Node*> Array_Expression::get_index_expressions(void)
{
    return this->indexExpressions;
}

Identifier* Array_Expression::get_identifier(void)
{
    return static_cast<Identifier*>(this->identifier);
}

Variable_Declaration::Variable_Declaration(Node* identifier, Node* type)
{
    set_name(AST_Node_Names::DECLARATION);
    this->identifier = identifier;
    this->type = type;
}

Identifier* Variable_Declaration::get_identifier(void)
{
    return static_cast<Identifier*>(this->identifier);
}

Primitive* Variable_Declaration::get_type(void)
{
    return static_cast<Primitive*>(this->type);
}

Variable_Assignment::Variable_Assignment(Node* identifier, Node* expression)
{
    set_name(AST_Node_Names::ASSIGNMENT);
    this->identifier = identifier;
    this->expression = expression;
}

Identifier* Variable_Assignment::get_identifier(void)
{
    return static_cast<Identifier*>(this->identifier);
}

Node* Variable_Assignment::get_expression(void)
{
    return this->expression;
}

Output::Output(std::vector<Node*> expressions)
{
    set_name(AST_Node_Names::OUTPUT);
    this->expressions = expressions;
}

std::vector<Node*> Output::get_expressions(void)
{
    return this->expressions;
}

Input::Input(Node* identifier)
{
    set_name(AST_Node_Names::INPUT);
    this->identifier = identifier;
}

Identifier* Input::get_identifier(void)
{
    return static_cast<Identifier*>(this->identifier);
}

Function::Function(Node* functionName, Node* returnType, Node* arguments, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::FUNCTION);
    this->functionName = functionName;
    this->returnType = returnType;
    this->arguments = arguments;
    this->statements = statements;
}

Identifier* Function::get_function_name(void)
{
    return static_cast<Identifier*>(this->functionName);
}

Function_Arguments* Function::get_arguments(void)
{
    return static_cast<Function_Arguments*>(this->arguments);
}

Primitive* Function::get_return_type(void)
{
    return static_cast<Primitive*>(this->returnType);
}

std::vector<Node*> Function::get_statements(void)
{
    return this->statements;
}

Function_Arguments::Function_Arguments(void)
{
    set_name(AST_Node_Names::FUNCTION_ARGUMENTS);
}

void Function_Arguments::add_argument(Node* identifier, Node* type)
{
    std::string identifierName = static_cast<Identifier*>(identifier)->get_variable_name();
    this->argumentNames.push_back(identifierName);
    this->argumentTypes.push_back(type);
    delete identifier;
    return;
}

std::vector<std::string> Function_Arguments::get_argument_names(void)
{
    return this->argumentNames;
}

std::vector<Node*> Function_Arguments::get_argument_types(void)
{
    return this->argumentTypes;
}

Function_Call::Function_Call(Node* functionName, Node* arguments)
{
    set_name(AST_Node_Names::FUNCTION_CALL);
    this->functionName = functionName;
    this->arguments = arguments;
}

Identifier* Function_Call::get_function_name(void)
{
    return static_cast<Identifier*>(this->functionName);
}

Call_Arguments* Function_Call::get_arguments(void)
{
    return static_cast<Call_Arguments*>(this->arguments);
}

Call_Arguments::Call_Arguments(void)
{
    set_name(AST_Node_Names::CALL_ARGUMENTS);
}

void Call_Arguments::add_argument(Node* expression)
{
    this->arguments.push_back(expression); 
}

std::vector<Node*> Call_Arguments::get_arguments(void)
{
    return this->arguments;
}

Return::Return(Node* expression)
{
    set_name(AST_Node_Names::RETURN);
    this->expression = expression;
}

Node* Return::get_expression(void)
{
    return this->expression;
}

While::While(Node* condition, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::WHILE);
    this->condition = condition;
    this->statements = statements;
}

Node* While::get_condition(void)
{
    return this->condition;
}

std::vector<Node*> While::get_statements(void)
{
    return this->statements;
}

For::For(Node* start, Node* end, Node* indexVariable, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::FOR);
    this->start = start;
    this->end = end;
    this->indexVariable = indexVariable;
    this->statements = statements;
    this->stepPresent = false;
}

For::For(Node* start, Node* end, Node* step, Node* indexVariable, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::FOR);
    this->start = start;
    this->end = end;
    this->step = step;
    this->indexVariable = indexVariable;
    this->statements = statements;
    this->stepPresent = true;
}

Node* For::get_start(void)
{
    return this->start;
}

Node* For::get_end(void)
{
    return this->end;
}

Node* For::get_step(void)
{
    return this->step;
}

Identifier* For::get_index_variable(void)
{
    return static_cast<Identifier*>(this->indexVariable);
}

std::vector<Node*> For::get_statements(void)
{
    return this->statements;
}

bool For::is_step_present(void)
{
    return this->stepPresent;
}

If::If(void)
{
    set_name(AST_Node_Names::IF);
}

If::If(Node* condition, std::vector<Node*> statements)
{
    set_name(AST_Node_Names::IF);
    this->condition = condition;
    this->statements = statements;
    this->elseNode = elseNode;
    this->elseNodePresent = false;
}

If::If(Node* condition, std::vector<Node*> statements, Node* elseNode)
{
    set_name(AST_Node_Names::IF);
    this->condition = condition;
    this->statements = statements;
    this->elseNode = elseNode;
    this->elseNodePresent = true;
}

Node* If::get_condition(void)
{
    return this->condition;
}

std::vector<Node*> If::get_statements(void)
{
    return this->statements;
}

bool If::is_else_node_present(void)
{
    return this->elseNodePresent;
}

Else* If::get_else_node(void)
{
    return static_cast<Else*>(this->elseNode);
}

Else::Else(void)
{
    set_name(AST_Node_Names::ELSE);
}

Else::Else(std::vector<Node*> statements)
{
    set_name(AST_Node_Names::ELSE);
    this->statements = statements;
}

std::vector<Node*> Else::get_statements(void)
{
    return this->statements;
}

Bracketed_Expression::Bracketed_Expression(Node* expression)
{
    set_name(AST_Node_Names::BRACKETED_EXPRESSION);
    this->expression = expression;
}

Node* Bracketed_Expression::get_expression(void)
{
    return this->expression;
}

Binary_Expression::Binary_Expression(Node* left, Token operation, Node* right)
{
    set_name(AST_Node_Names::BINARY_EXPRESSION);
    this->left = left;
    this->operation = operation;
    this->right = right;
}

Node* Binary_Expression::get_left_expression(void)
{
    return this->left;
}

Token Binary_Expression::get_operator(void)
{
    return this->operation;
}

Node* Binary_Expression::get_right_expression(void)
{
    return this->right;
}

Unary_Expression::Unary_Expression(Node* expression, Token operation)
{
    set_name(AST_Node_Names::UNARY_EXPRESSION);
    this->expression = expression;
    this->operation = operation;
}

Node* Unary_Expression::get_expression(void)
{
    return this->expression;
}

Token Unary_Expression::get_operator(void)
{
    return this->operation;
}