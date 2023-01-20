#ifndef Token_H
#define Token_H

typedef struct Token_t
{
  std::string type;
  std::string value;
  Token_t(std::string type, std::string value)
  {
    this->type = type;
    this->value = value;
  }
  Token_t(std::string type, char value)
  {
    this->type = type;
    this->value = value;
  }
  Token_t()
  {
    this->type = "";
    this->value = "";
  }
} Token;

#endif