#ifndef Exceptions_H
#define Exceptions_H

#include <iostream>
#include <string>
#include "constants.h"

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
        Generic_Error(void);
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

#endif