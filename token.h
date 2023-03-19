#ifndef Token_H
#define Token_H

typedef struct Token_T
{
  std::string type; 
  std::string value;
  Token_T(std::string type, std::string value)
  {
    this->type = type;
    this->value = value;
  }
  Token_T(std::string type, char value)
  {
    this->type = type;
    this->value = value;
  }
  Token_T()
  {
    this->type = "";
    this->value = "";
  }
} Token;

#endif