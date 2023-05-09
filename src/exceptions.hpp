#ifndef Exceptions_Hpp
#define Exceptions_Hpp

#include <iostream>
#include <string>
#include "constants.hpp"

class Error
{
    private:
        std::string TYPE;
    public:
        virtual void display_problem(void) = 0;
        std::string get_type(void);
        void set_type(std::string type);
};

class Generic_Error: public Error
{
    public:
        std::string errorMessage;
        Generic_Error(std::string errorMessage);
        void display_problem(void);
};

class Lexical_Error: public Error
{
    private:
        std::string errorLine;
    public:
        Lexical_Error(std::string errorLine);
        void display_problem(void);
};

class Unexpected_Parsing_Input: public Error
{
    private:
        std::string received;
        std::string expected;
    public:
        Unexpected_Parsing_Input(std::string received, std::string expected);
        void display_problem(void);
};

class Missed_Terminating_Token: public Error
{
    private:
        std::string expected;
    public:
        Missed_Terminating_Token(std::string expected);
        void display_problem(void);
};

class Reached_End_Of_File: public Error  // signal to end the parsing process
{
    public:
        Reached_End_Of_File(void);
        void display_problem(void);
};

class Type_Mismatch: public Error
{
    private:
        std::string expectedType;
        std::string receivedType;
    public:
        Type_Mismatch(std::string expectedType, std::string receivedType);
        void display_problem(void);
};

class Undeclared_Variable: public Error
{
    private:
        std::string variableName;
    public:
        Undeclared_Variable(std::string variableName);
        void display_problem(void);
};

class Improper_Keyword_Placement: public Error
{
    private:
        std::string keyword;
    public:
        Improper_Keyword_Placement(std::string keyword);
        void display_problem(void);
};

class Missing_Keyword: public Error
{
    private:
        std::string keyword;
    public:
        Missing_Keyword(std::string keyword);
        void display_problem(void);
};

class Redeclaration: public Error
{
    private:
        std::string variableName;
    public:
        Redeclaration(std::string variableName);
        void display_problem(void);
};

#endif