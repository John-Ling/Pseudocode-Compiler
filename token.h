struct Token
{
    std::string name;
    std::string value;
    Token(std::string name, std::string value)
    {
        this->name = name;
        this->value = value;
    }
};