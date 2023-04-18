#ifndef Exceptions_H
#define Exceptions_H

#include <iostream>
#include <string>

class Error
{
    public:
        virtual void display_problem(std::string errorLine) = 0;
};

class Lexical_Error: Error
{
    public:
        void display_problem(std::string errorLine);
};

class Parse_Error: Error
{
    public:
        void display_problem(std::string errorLine);
};

class Type_Error: Error
{
    public:
        void display_problem(std::string errorLine);
};

class Reached_End_Of_File {}; // signal to end parsing process


#endif