#pragma once

#include "Json/Node/Node.hpp"
#include "Json/Parser/Parser.hpp"
#include <fstream>
#include <iostream>

namespace Json
{
inline void WriteFile(const std::string &path, const Node &node)
{
    try
    {
        std::ofstream file;
        file.open(path);
        file << node;
        file.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "failed to write " << path << " : " << e.what() << std::endl;
    }
}
} // namespace Json