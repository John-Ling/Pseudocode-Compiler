#include "code_generator.h"

Code_Generator::Code_Generator(std::string filename, std::vector<Node> nodes)
{
    // remove .pcode file extension
    std::string editedFilename;
    for (unsigned int i = 0; i < filename.size(); i++)
    {
        if (filename[i] != '.')
        {
            editedFilename += filename[i];
        }
        else
        {
            break;
        }
    }
    this->targetFilename = editedFilename;
    this->nodes = nodes;
}

int Code_Generator::generate_code(void)
{
    std::ofstream targetFile(this->targetFilename + ".py");
    targetFile.close();
    return 0;
}

