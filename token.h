struct Token
{
  std::string type;
  std::string value;
  Token(std::string type, std::string value)
  {
    this->type = type;
    this->value = value;
  }
  Token(std::string type, char value)
  {
    this->type = type;
    this->value = value;
  }
  Token()
  {
    this->type = "";
    this->value = "";
  }
};