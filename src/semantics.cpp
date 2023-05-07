#include "semantics.hpp"

Semantic_Analyser::Semantic_Analyser(std::vector<Node*> nodes, std::unordered_map<std::string, std::string> symbolTable)
{
    this->nodes = nodes;
    this->symbolTable = symbolTable;
}

int Semantic_Analyser::semantic_analysis(void)
{
    for (unsigned int i = 0; i < this->nodes.size(); i++)
    {
        examine(this->nodes[i]);
        set_state(""); // reset state of analyser
    }
    return 0;
}

void Semantic_Analyser::examine(Node* node)
{
    static std::unordered_map<std::string, nodeFunctionPointer> nodeMap = {
        {AST_Node_Names::ASSIGNMENT, &Semantic_Analyser::variable_assignment}, {AST_Node_Names::FUNCTION, &Semantic_Analyser::function},
        {AST_Node_Names::DECLARATION, &Semantic_Analyser::variable_declaration}, {AST_Node_Names::RETURN, &Semantic_Analyser::return_}
    };

    if (nodeMap.count(node->get_node_name()))
    {
        try
        {
            (this->*nodeMap[node->get_node_name()])(node);
        }
        catch(Error &e)
        {
            e.display_problem();
            exit(1);
        }
    }
}

void Semantic_Analyser::set_state(std::string state)
{
    this->analyserState = state;
    return;
}

// Set declaration status of identifier node to true
void Semantic_Analyser::variable_declaration(Node* node)
{
    Variable_Declaration* declaration = static_cast<Variable_Declaration*>(node);
    Identifier* identifier = declaration->get_identifier();
    
    if (identifier->get_declared() == true) // if identifier has already been declared
    {
        throw Redeclaration(identifier->get_variable_name());
    }
    identifier->set_declared(true);
    return;
}

void Semantic_Analyser::function(Node* node)
{
    set_state(Analyser_States::FUNCTION);
    Function* functionNode = static_cast<Function*>(node);
    std::vector<Node*> statements = functionNode->get_statements();

    bool returnStatementFound = false; // all functions must have a return statement
    for (unsigned int i = 0; i < statements.size(); i++)
    {
        if (statements[i]->get_node_name() == AST_Node_Names::RETURN)
        {
            returnStatementFound = true;
        }
        else
        {
            examine(statements[i]);
        }
    }
    
    if (!returnStatementFound)
    {
        throw Missing_Keyword(Keywords::RETURN);
    }

    set_state("");
    return;
}

void Semantic_Analyser::return_(Node* node)
{
    node->get_node_name(); // placed to remove compiler warning
    if (this->analyserState != Analyser_States::FUNCTION) // return statements can only exist in functions
    {
        throw Improper_Keyword_Placement(Keywords::RETURN);
    }
    return;
}

// Determines the datatype of both the variable being assigned to and the assignment expression then compares them. Type mismatch is thrown if types do not match
void Semantic_Analyser::variable_assignment(Node* node)
{   
    Variable_Assignment* assignment = static_cast<Variable_Assignment*>(node);
    Identifier* identifier = assignment->get_identifier();
    if (!this->symbolTable.count(identifier->get_variable_name()))
    {
        throw Undeclared_Variable(identifier->get_variable_name());
    }

    std::string identifierType = this->symbolTable[identifier->get_variable_name()];
    
    // determine expression type
    Node* expression = assignment->get_expression();

    std::string expressionType;
    try
    {
        expressionType = typecheck_expression(expression);
    }
    catch(Error& e)
    {
        e.display_problem();
        exit(1);
    }
    
    if (expressionType != identifierType)
    {
        throw Type_Mismatch(identifierType, expressionType);
    }

    return;
}

// determine output of expression 

// Gets the datatypes of the operands of an expression, checks if they are the same type then returns the identical type

std::string Semantic_Analyser::typecheck_expression(Node* node)
{
    // base case
    if (node->get_node_name() == AST_Node_Names::LITERAL || node->get_node_name() == AST_Node_Names::IDENTIFIER || node->get_node_name() == AST_Node_Names::FUNCTION_CALL)
    {
        // return value of literal or datatype of function call or datatype of identifier
        if (node->get_node_name() == AST_Node_Names::LITERAL)
        {
            return this->LITERAL_TO_PRIMITIVE.at(static_cast<Literal*>(node)->get_type());
        }
        else if (node->get_node_name() == AST_Node_Names::IDENTIFIER)
        {
            if (this->symbolTable.count(static_cast<Identifier*>(node)->get_variable_name()))
            {
                return this->symbolTable[static_cast<Identifier*>(node)->get_variable_name()];
            }
            throw Generic_Error();
        }
        else if (node->get_node_name() == AST_Node_Names::FUNCTION_CALL)
        {
            std::string functionName = static_cast<Function_Call*>(node)->get_function_name()->get_variable_name();

            if (this->symbolTable.count(functionName))
            {
                return this->symbolTable[functionName];
            }
            throw Generic_Error();
        }
    }

    static std::string expressionType;
    // get type of expression and react accordingly (i.e binary expressions runs typecheck() twice for left and right expressions) 
    if (node->get_node_name() == AST_Node_Names::BINARY_EXPRESSION)
    {
        Token operation = static_cast<Binary_Expression*>(node)->get_operator();
        std::vector<std::string> types = {typecheck_expression(static_cast<Binary_Expression*>(node)->get_left_expression()), typecheck_expression(static_cast<Binary_Expression*>(node)->get_right_expression())};

        match_types(types);
        if (this->LITERAL_TO_PRIMITIVE.count(types[0]))
        {
            expressionType = this->LITERAL_TO_PRIMITIVE.at(types[0]);
        }
        else
        {
            expressionType = types[0];
        }
    }
    else if (node->get_node_name()== AST_Node_Names::UNARY_EXPRESSION)
    {
        Token operation = static_cast<Unary_Expression*>(node)->get_operator();
        std::vector<std::string> types = {typecheck_expression(static_cast<Unary_Expression*>(node)->get_expression())};

        match_types(types);
        if (this->LITERAL_TO_PRIMITIVE.count(types[0]))
        {
            expressionType = this->LITERAL_TO_PRIMITIVE.at(types[0]);
        }
        else
        {
            expressionType = types[0];
        }
    }
    else if (node->get_node_name() == AST_Node_Names::BRACKETED_EXPRESSION)
    {
        typecheck_expression(static_cast<Bracketed_Expression*>(node)->get_expression());
    }

    return expressionType;
}

// Receives a vector of primitive/literal tokens and checks if they are all the same. Returns true if so.
void Semantic_Analyser::match_types(std::vector<std::string> types)
{
    unsigned int index = 1;
    while (index < types.size())
    {
        std::string currentType = types[index];
        std::string previousType = types[index - 1];

        if (this->LITERAL_TO_PRIMITIVE.count(currentType))
        {
            currentType = this->LITERAL_TO_PRIMITIVE.at(currentType);
        }

        if (this->LITERAL_TO_PRIMITIVE.count(previousType))
        {
            previousType = this->LITERAL_TO_PRIMITIVE.at(previousType);
        }

        if (currentType != previousType)
        {
            throw Type_Mismatch(currentType, previousType);
        }
        index++;
    }
    return;
}