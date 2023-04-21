#ifndef Exceptions_H
#define Exceptions_H

#include <iostream>
#include <string>

class Error
{
    public:
        virtual void display_problem(void) = 0;
};

class Lexical_Error: Error
{
    private:
        std::string errorLine;
    public:
        Lexical_Error(std::string errorLine);
        void display_problem(void);
};

class Unexpected_Parsing_Input: Error
{
    private:
        std::string received;
        std::string expected;
    public:
        Unexpected_Parsing_Input(std::string received, std::string expected);
        void display_problem(void);
};

class Wrong_AST_Type: Error
{
    private:
        std::string received;
    public:
        Wrong_AST_Type(std::string received);
        void display_problem(void);
        std::string get_received(void);
};

class Missed_Terminating_Token: Error
{
    private:
        std::string expected;
    public:
        Missed_Terminating_Token(std::string expected);
        void display_problem(void);
};

class Type_Error: Error
{
    public:
        void display_problem(std::string errorLine);
};

class Reached_End_Of_File {}; // signal to end parsing process


#endif