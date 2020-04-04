/**
  * @file
  * @brief Содержит определения вспомогательных функций, описанных в functions.h
  */

#include "functions.h"
#include <fstream>
#include <string>
#include <vector>

using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;

Data data_from_csv(const std::string& filename, char sep)
{
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Cannot open input csv-file");
    std::string line;
    Data vec;
    while(std::getline(f, line))
    {
        vec.push_back(get_line_from_csv(line, sep));
    }
    f.close();
    return vec;
}

std::string get_file_ext(const std::string& filename)
{
    size_t i = filename.rfind('.', filename.length());
    if (i != std::string::npos)
        return(filename.substr(i+1, filename.length() - i));
    return("");
}


