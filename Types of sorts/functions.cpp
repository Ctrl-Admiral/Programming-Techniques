/**
  * @file
  * @brief Файл исходного кода, содержащий определения функций, описанных в functions.h
  */

#include "sorts.h"
#include "functions.h"
#include <chrono>
#include <fstream>
#include <functional> // std::function
#include <map>
#include <string>
#include <vector>

using Data = std::vector<Entry>;
using Clock = std::chrono::high_resolution_clock;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;

Data data_from_csv(const std::string& filename, char sep)
{
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Cannot open output csv-file");
    std::string line;
    Data vec;
    while(std::getline(f, line))
    {
        vec.push_back(get_line_from_csv(line, sep));
    }
    f.close();
    return vec;
}

void data_to_csv(const std::string& filename, const Data& data, char sep)
{
    std::ofstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Cannot open output csv-file");

    for (Entry elem : data)
    {
        elem.to_csv(f, sep);
    }
    f.close();
}

void times_to_csv(const std::string& filename, const std::vector<my_tuple>& statistics, char sep)
{
    std::ofstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Cannot open output csv-file");

    for (const my_tuple& elem : statistics)
    {
        const auto& [size, sort, t] = elem;
        f << size << sep << sort << sep << t << '\n';
    }
    f.close();
}

Data get_slice_of_data(const Data& data, const std::size_t& size)
{
    // copying data
    Data::const_iterator last = data.begin() + static_cast<std::ptrdiff_t>(size);
    Data part_data = Data(data.begin(), last);
    return part_data;
}

std::uint64_t get_time_sort(const std::function<void(Data::iterator, Data::iterator)>& sort_function,
                          const Data& data, const std::size_t& size)
{
    Data part_data = get_slice_of_data(data, size);

    std::chrono::time_point<Clock> start = Clock::now();
    sort_function(part_data.begin(), part_data.end());
    std::chrono::time_point<Clock> end = Clock::now();
    //std::cout << "Is it sorted:" <<std::is_sorted(data.begin(), data.end()) << "\n";
    //data_to_csv("sorted.csv", part_data);
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

std::string get_file_ext(const std::string& filename)
{
    size_t i = filename.rfind('.', filename.length());
    if (i != std::string::npos)
        return(filename.substr(i+1, filename.length() - i));
    return("");
}
