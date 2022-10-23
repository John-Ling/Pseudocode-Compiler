#ifndef Compiler_H
#define Compiler_H
#include <string>
#include <vector>
#include <map>

class Compiler
{    
    private:
        std::vector<std::string> tokens;
    public:
        std::string targetFile;
        const std::string SYMBOLS = "()*/+-,%><[]"; // acceptable symbols in the language
        Compiler(std::string filename);
        void tokenize(std::string line);
};

#endif