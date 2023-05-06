#include "code_generator.h"

Code_Generator::Code_Generator(std::string filename, std::vector<Node*> nodes,  std::unordered_map<std::string, std::string> identifierTable)
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
    this->indentation = "";
    this->identifierTable = identifierTable;
}

// Iterate through vector of nodes and write generated code to the output file
int Code_Generator::generate_code(void)
{
    std::ofstream targetFile(this->targetFilename + ".py");
    for (unsigned int i = 0; i < this->nodes.size(); i++)
    {
        std::string generated = examine(this->nodes[i]);
        if (generated != "")
        {
            targetFile << generated + '\n';
        }
    }
    targetFile.close();
    return 0;
}

// Examines the node the attribute pointer points to and returns correct python code as a string.
std::string Code_Generator::examine(Node* node)
{
    std::string nodeName = node->get_node_name();
    if (nodeMap.count(nodeName))
    {
        std::string generated = (this->*nodeMap[nodeName])(node);
        return generated; // call and return function pointer
    }
    else
    {
        return "";
    }
}

std::string Code_Generator::block_statement(std::vector<Node*> statements)
{   
    std::string generated = "";
    this->indentation += '\t'; // add indentation
    for (unsigned int i = 0; i < statements.size(); i++) // generate code within block statement with correct indentation
    {
        std::string tmp = (this->*nodeMap[statements[i]->get_node_name()])(statements[i]);
        if (tmp != "")
        {
            generated += this->indentation + tmp + '\n';
        }
    }
    this->indentation.pop_back(); // remove indentation
    return generated;
}

std::string Code_Generator::primitive(Node* node)
{
    std::string generated = "";
    std::string type = static_cast<Primitive*>(node)->get_type();
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
    delete node;
    return generated;
}

std::string Code_Generator::primitive(std::string type)
{
    std::string generated;
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
    delete convertedNode;
    return generated;
}

std::string Code_Generator::identifier(Node* node)
{
    std::string identifierName = static_cast<Identifier*>(node)->get_variable_name();
    delete node;
    return identifierName;
}

std::string Code_Generator::convert_operator(Token token)
{
    static std::unordered_map<std::string, std::string> operatorMap = {
        {Tokens::MOD, "%"}, {Tokens::NOT, "not "}, {Tokens::EQUAL, "=="}, 
        {Tokens::NOT_EQUAL, "!="}, {Tokens::AND, " and "}, {Tokens::OR, " or "}, {Tokens::ASSIGNMENT, "="}
    };

    if (operatorMap.count(token.type))
    {
        return operatorMap[token.type];
    }
    return token.value;
}

std::string Code_Generator::function(Node* node)
{
    std::string generated = "def ";
    Function* convertedNode = static_cast<Function*>(node);
    Identifier* functionName = convertedNode->get_function_name();
    Primitive* returnType = convertedNode->get_return_type();
    Function_Arguments* functionArguments = convertedNode->get_arguments();
    std::vector<std::string> argumentNames = functionArguments->get_argument_names();
    std::vector<Node*> argumentTypes = functionArguments->get_argument_types();

    generated += functionName->get_variable_name() + '(';
    for (unsigned int i = 0; i < argumentNames.size(); i++) // generate function arguments
    {
        generated += argumentNames[i] + ": " + primitive(argumentTypes[i]) + ", ";
    }

    generated.pop_back(); // remove space
    generated.pop_back(); // remove last comma
    generated += ") -> " + primitive(returnType) + ":\n";
    std::vector<Node*> statements = convertedNode->get_statements();
    generated += block_statement(statements);

    delete functionArguments;
    delete functionName;
    delete convertedNode;
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

    delete convertedNode;
    return generated;
}

std::string Code_Generator::if_(Node* node)
{
    std::string generated = "if ";
    If* convertedNode = static_cast<If*>(node);
    std::cout << convertedNode->get_condition()->get_node_name();
    generated += examine(convertedNode->get_condition()) + ":\n";

    std::vector<Node*> statements = convertedNode->get_statements();
    generated += block_statement(statements);
    if (convertedNode->is_else_node_present())
    {
        generated += this->indentation + "else:\n";
        Else* elseNode = static_cast<Else*>(convertedNode->get_else_node());
        std::vector<Node*> statements = elseNode->get_statements();
        generated += block_statement(statements);
        delete elseNode;
    }

    delete convertedNode;
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
    
    delete indexVariable;
    delete convertedNode;

    return generated;
}

std::string Code_Generator::return_(Node* node)
{
    std::string generated = "return " + examine(static_cast<Return*>(node)->get_expression());
    delete node;
    return generated;
}

std::string Code_Generator::variable_declaration(Node* node) // python is not strongly typed so variable declarations are not needed
{
    std::cout << "Variable Declaration\n";
    Variable_Declaration* convertedNode = static_cast<Variable_Declaration*>(node);
    delete convertedNode->get_identifier();
    delete convertedNode->get_type();
    delete convertedNode;
    return "";
}

std::string Code_Generator::variable_assignment(Node* node)
{
    std::cout << "Assignment\n";
    Variable_Assignment* convertedNode = static_cast<Variable_Assignment*>(node);
    Identifier* variable = convertedNode->get_identifier();
    Node* expression = convertedNode->get_expression();
    std::string generated = variable->get_variable_name() + " = " + examine(expression);

    delete variable;
    delete convertedNode;
    return generated;
}

std::string Code_Generator::output(Node* node)
{
    std::cout << "Output\n";
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
    delete convertedNode;
    return generated;
}

std::string Code_Generator::input(Node* node)
{
    Input* convertedNode = static_cast<Input*>(node);
    Identifier* identifier = convertedNode->get_identifier();
    std::string variableName = identifier->get_variable_name();
    std::string type = primitive(this->identifierTable[variableName]);
    std::string generated = variableName + " = " + type + "(input())";
    
    delete identifier;
    delete convertedNode;
    return generated;
}

std::string Code_Generator::bracketed_expression(Node* node)
{
    Bracketed_Expression* convertedNode = static_cast<Bracketed_Expression*>(node);

    std::string generated = "(" + examine(convertedNode->get_expression()) + ")";

    delete convertedNode;
    return generated;
}

std::string Code_Generator::binary_expression(Node* node)
{
    std::cout << "Binary Expression\n";
    Binary_Expression* convertedNode = static_cast<Binary_Expression*>(node);
    Node* leftExpression = convertedNode->get_left_expression();

    Node* rightExpression = convertedNode->get_right_expression();
    Token operation = convertedNode->get_operator();
    std::string generated = examine(leftExpression) + convert_operator(operation) + examine(rightExpression);

    delete convertedNode;
    return generated;
}

std::string Code_Generator::unary_expression(Node* node)
{
    std::cout << "Unary Expression\n";
    Unary_Expression* convertedNode = static_cast<Unary_Expression*>(node);
    Token operation = convertedNode->get_operator();
    Node* expression = convertedNode->get_expression();
    std::string generated = convert_operator(operation) + examine(expression);

    delete convertedNode;
    return generated;
}

std::string Code_Generator::function_call(Node* node)
{
    Function_Call* convertedNode = static_cast<Function_Call*>(node);
    Identifier* functionName = convertedNode->get_function_name();
    std::string generated = functionName->get_variable_name() + '(';

    Call_Arguments* arguments = convertedNode->get_arguments();
    std::vector<Node*> argumentVector = arguments->get_arguments();
    for (unsigned int i = 0; i < argumentVector.size(); i++)
    {
        std::string argument = examine(argumentVector[i]);
        generated += argument + ", ";
    }
    generated.pop_back();
    generated.pop_back();
    generated += ')';

    delete functionName;
    delete arguments;
    delete convertedNode;
    return generated;
}