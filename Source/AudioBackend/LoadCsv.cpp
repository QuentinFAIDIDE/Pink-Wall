#include "LoadCsv.h"

#include <algorithm>
#include <fstream>

std::vector<float> loadCsv(std::string path)
{

    // counting file lines
    // (https://stackoverflow.com/questions/3072795/how-to-count-lines-of-a-file-in-c)
    std::ifstream inFile(path);
    int noLines = std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n');

    // allocating response array
    std::vector<float> response;
    response.reserve(noLines);

    inFile.clear();
    inFile.seekg(0);

    for (std::string line; getline(inFile, line);)
    {
        if (line == "")
        {
            break;
        }
        size_t tokenPos = line.find(',');
        std::string secondColumn = line.substr(tokenPos + 1, line.size() - (tokenPos + 1));
        float parsedFloat = std::stof(secondColumn);
        response.push_back(parsedFloat);
    }

    return response;
}