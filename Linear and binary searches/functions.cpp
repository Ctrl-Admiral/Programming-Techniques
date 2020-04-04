/**
  * @file
  * @brief Файл исходного кода, содержащий определения функций, описанных в functions.h
  */

#include "quick.h"
#include "functions.h"
#include "my_searches.h"
#include <chrono>
#include <fstream>
#include <functional> // std::function
#include <map>
#include <string>
#include <vector>


using Data = std::vector<Entry>;
using Clock = std::chrono::high_resolution_clock;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;
using mmap_name_entry = std::multimap<Entry::Name, Entry>;

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
        const auto& [ size, sort, t ] = elem;
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

std::uint64_t get_time_lin_search(const Entry::Name& key, const Data& data, const std::size_t& size)
{
    Data::const_iterator data_size = std::next(data.begin(), static_cast<std::ptrdiff_t>(size));
    std::chrono::time_point<Clock> start = Clock::now();
    auto _ = study::linear_search(data.begin(), data_size, key, [](const Entry& lhs, const Entry::Name& rhs){return lhs.getName() == rhs;});
    std::chrono::time_point<Clock> end = Clock::now();

    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
}

std::uint64_t get_time_bin_search(const Entry::Name& key, const Data& data,
                                  const std::size_t& size, bool time_before_sort)
{
    std::chrono::time_point<Clock> start;
    std::chrono::time_point<Clock> end;
    Data part_data = get_slice_of_data(data, size);

    if (time_before_sort)
    {
        start = Clock::now();
        study::q_sort(part_data.begin(), part_data.end());

    }
    else
    {
        study::q_sort(part_data.begin(), part_data.end());
        start = Clock::now();
    }
    auto _ = study::binary_search(part_data.begin(),
                         part_data.end(), key,
                         [](const std::string& lhs, const std::string& rhs) {return lhs < rhs;},
                         [](const Entry& elem) {return elem.getName();});
    end = Clock::now();

    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
}

std::uint64_t get_time_mmap_search(const Entry::Name& key, const Data& data,
                                   const std::size_t& size)
{
    Data part_data = get_slice_of_data(data, size);
    mmap_name_entry mmap_Entry = data_to_map(part_data);

    std::chrono::time_point<Clock> start = Clock::now();
    auto _ = mmap_Entry.equal_range(key);
    std::chrono::time_point<Clock> end = Clock::now();

    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
}

std::string get_file_ext(const std::string& filename)
{
    size_t i = filename.rfind('.', filename.length());
    if (i != std::string::npos)
        return(filename.substr(i+1, filename.length() - i));
    return("");
}

mmap_name_entry data_to_map(const Data& data)
{
    mmap_name_entry mmap_Entry;
    for (const auto& elem : data)
    {
        mmap_Entry.emplace(std::pair<Entry::Name, Entry>(elem.getName(), elem));
    }
    return mmap_Entry;
}
