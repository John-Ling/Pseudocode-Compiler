#include "code_generator.h"

Code_Generator::Code_Generator(std::string filename, std::vector<Node*> nodes)
{
    // remove .pcode file extension
    std::string editedFilename;
    for (unsigned int i = 0; i < filename.size(); i++)
    {
        if (filename[i] == '.')
        {
            break;
        }
        editedFilename += filename[i];
    }

    this->targetFilename = editedFilename;
    this->nodes = nodes;
    this->pointer = 0;
}

void Code_Generator::advance(void)
{
    if (this->pointer == -1 || this->pointer + 1 >= (int)this->nodes.size())
    {
        this->pointer = -1;
        return;
    }

    this->pointer++;
}

// Iterate through vector of nodes and write generated code to the output file
int Code_Generator::generate_code(void)
{
    std::ofstream targetFile(this->targetFilename + ".py");
    for (unsigned int i = 0; i < this->nodes.size(); i++)
    {
        std::string generated = examine(this->nodes[i]) + '\n';
        targetFile << generated;
    }
    targetFile.close();
    return 0;
}

// Examines the node the attribute pointer points to and returns correct python code as a string.
std::string Code_Generator::examine(Node* node)
{
    static std::unordered_map<std::string, nodeFunctionPointer> nodeMap = { // unordered map of function pointers to quickly select the correct procedure based on the node's name
        {AST_Node_Names::FUNCTION, &Code_Generator::function},
        {AST_Node_Names::WHILE, &Code_Generator::while_}, {AST_Node_Names::IF, &Code_Generator::if_}, {AST_Node_Names::FOR, &Code_Generator::for_},
        {AST_Node_Names::RETURN, &Code_Generator::return_}, {AST_Node_Names::LITERAL, &Code_Generator::literal},
        {AST_Node_Names::IDENTIFIER, &Code_Generator::identifier}, {AST_Node_Names::DECLARATION, &Code_Generator::variable_declaration}, 
        {AST_Node_Names::ASSIGNMENT, &Code_Generator::variable_assignment}, {AST_Node_Names::OUTPUT, &Code_Generator::output}, 
        {AST_Node_Names::INPUT, &Code_Generator::input}, {AST_Node_Names::BINARY_EXPRESSION, &Code_Generator::binary_expression},
        {AST_Node_Names::UNARY_EXPRESSION, &Code_Generator::unary_expression}, {AST_Node_Names::FUNCTION_CALL, &Code_Generator::function_call}};

    std::string nodeName = node->get_node_name();
    return (this->*nodeMap[nodeName])(node); // call and return function pointer
}

std::string Code_Generator::block_statement(std::vector<Node*> statements)
{
    std::string generated = "";
    for (unsigned int i = 0; i < statements.size(); i++) // generate code within block statement
    {
        generated += '\t' + examine(statements[i]) + '\n';
    }
    return generated;
}

std::string Code_Generator::primitive(Node* node)
{
    std::string generated = "";
    std::cout << "Changing\n";
    std::string type = static_cast<Primitive*>(node)->get_type();
    std::cout << "Typecasted\n";
    if (type == Tokens::INTEGER)
    {
        generated = "int";
    }
    else if (type == Tokens::FLOAT)
    {
        generated = "float";
    }
    else if (type == Tokens::STRING)
    {
        generated = "str";
    }
    else if (type == Tokens::BOOLEAN)
    {
        generated = "bool";
    }
    return generated;
}

std::string Code_Generator::literal(Node* node)
{
    std::string generated = "";
    Literal* convertedNode = static_cast<Literal*>(node);
    if (convertedNode->get_type() == Tokens::BOOLEAN_LITERAL)
    {
        if (convertedNode->get_value() == Keywords::TRUE)
        {
            generated = "True";
        }
        else
        {
            generated = "False";
        }
    }
    else
    {
        generated = convertedNode->get_value();
    }
    return generated;
}

std::string Code_Generator::identifier(Node* node)
{
    std::string generated = "";
    Identifier* identifier = static_cast<Identifier*>(node);
    generated = identifier->get_variable_name();
    return generated;
}

std::string Code_Generator::convert_operator(Token token)
{
    static std::unordered_map<std::string, std::string> operatorMap = {
        {Tokens::ADDITION, "+"}, {Tokens::SUBTRACTION, "-"}, {Tokens::MULTIPLICATION, "*"},
        {Tokens::DIVISION, "/"}, {Tokens::MOD, "%"}, {Tokens::NOT, "not"}};
    return operatorMap[token.type];
}

std::string Code_Generator::function(Node* node)
{
    std::string generated = "def ";
    std::cout << "Flag 1\n";
    Function* convertedNode = static_cast<Function*>(node);
    Identifier* functionName = convertedNode->get_function_name();
    generated += functionName->get_variable_name();
    Primitive* returnType = convertedNode->get_return_type();
    Function_Arguments* functionArguments = convertedNode->get_arguments();

    std::cout << "Flag 2\n";
    std::string arguments = "(";
    for (auto i : functionArguments->get_arguments()) // iterate through function arguments and generate code
    {
        std::cout << "Item " << i.first << '\n';
        arguments += i.first + ": " + primitive(i.second) + ", "; // add variable name
    }

    arguments.pop_back(); // remove space
    arguments.pop_back(); // remove last comma
    std::cout << "Generating return type\n";
    arguments += ") -> " + primitive(returnType) + ":\n";
    std::cout << "Done\n";
    generated += arguments;
    std::vector<Node*> statements = convertedNode->get_statements();

    std::cout << "Flag 3\n";
    generated += block_statement(statements);
    return generated;
}

std::string Code_Generator::while_(Node* node)
{
    std::string generated = "while ";
    While* convertedNode = static_cast<While*>(node);
    Node* condition = convertedNode->get_condition();
    generated += examine(condition) + ":\n";

    std::vector<Node*> statements = convertedNode->get_statements();
    generated += block_statement(statements);
    return generated;
}

std::string Code_Generator::if_(Node* node)
{
    std::string generated = "if ";
    If* convertedNode = static_cast<If*>(node);
    generated += examine(convertedNode->get_condition()) + ":\n";

    std::vector<Node*> statements = convertedNode->get_statements();
    generated += block_statement(statements);

    if (convertedNode->is_else_node_present())
    {
        generated += "else:\n";
        Else* elseNode = static_cast<Else*>(convertedNode->get_else_node());
        std::vector<Node*> statements = elseNode->get_statements();
        generated += block_statement(statements);
    }

    return generated;
}

std::string Code_Generator::for_(Node* node)
{
    std::string generated = "for ";
    For* convertedNode = static_cast<For*>(node);
    Identifier* indexVariable = convertedNode->get_index_variable();
    Node* start = convertedNode->get_start();
    Node* end = convertedNode->get_end();

    std::string step = "";
    if (convertedNode->is_step_present())
    {
        Node* stepExpression = convertedNode->get_step();
        step += ", " + examine(stepExpression);
    }

    generated += indexVariable->get_variable_name() + ' ' + "in range(" + examine(start) + ", " + examine(end) + step + "):\n";
    std::vector<Node*> statements = convertedNode->get_statements();
    generated += block_statement(statements);
    return generated;
}

std::string Code_Generator::return_(Node* node)
{
    std::string generated = "return ";
    return generated + examine(static_cast<Return*>(node)->get_expression());
}

std::string Code_Generator::variable_declaration(Node* node) // python is not strongly typed so variable declarations are not needed
{
    return "";
}

std::string Code_Generator::variable_assignment(Node* node)
{
    std::string generated = "";
    Variable_Assignment* convertedNode = static_cast<Variable_Assignment*>(node);
    Identifier* variable = convertedNode->get_identifier();
    Node* expression = convertedNode->get_expression();
    generated = variable->get_variable_name() + " = " + examine(expression);
    return generated;
}

std::string Code_Generator::output(Node* node)
{
    std::string generated = "print(";
    Output* convertedNode = static_cast<Output*>(node);
    std::vector<Node*> expressions = convertedNode->get_expressions();

    if (expressions.size() != 0)
    {
        for (unsigned int i = 0; i < expressions.size(); i++) 
        {
            generated += examine(expressions[i]) + ", ";    
        }
        generated.pop_back(); // remove space and comma
        generated.pop_back();
    }
    generated += ')';
    return generated;
}

std::string Code_Generator::input(Node* node)
{
    std::string generated = "";
    Input* convertedNode = static_cast<Input*>(node)   ;
    Identifier* identifier = convertedNode->get_identifier();
    generated = identifier->get_variable_name() + " = " + "input()";
    return generated;
}

std::string Code_Generator::binary_expression(Node* node)
{
    std::string generated = "";
    Binary_Expression* convertedNode = static_cast<Binary_Expression*>(node);
    Node* leftExpression = convertedNode->get_left_expression();
    Node* rightExpression = convertedNode->get_right_expression();
    Token operation = convertedNode->get_operator();
    generated = examine(leftExpression) + convert_operator(operation) + examine(rightExpression);
    return generated;
}

std::string Code_Generator::unary_expression(Node* node)
{
    std::string generated = "";
    Unary_Expression* convertedNode = static_cast<Unary_Expression*>(node);
    Token operation = convertedNode->get_operator();
    Node* expression = convertedNode->get_expression();
    generated = convert_operator(operation) + '(' + examine(expression) + ')';
    return generated;
}

std::string Code_Generator::function_call(Node* node)
{
    std::string generated = "";
    Function_Call* convertedNode = static_cast<Function_Call*>(node);
    Identifier* functionName = convertedNode->get_function_name();
    generated = functionName->get_variable_name() + '(';

    std::vector<Node*> arguments = convertedNode->get_arguments();
    for (unsigned int i = 0; i < arguments.size(); i++)
    {
        std::string argument = examine(arguments[i]);
        generated += argument + ", ";
    }
    generated.pop_back();
    generated.pop_back();
    generated += ')';
    return generated;
}