#include "functions.h"
#include "my_searches.h"
#include <iostream>
#include <map>
#include <numeric> //for std::accumulate
#include <vector>


using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;
using mmap_name_entry = std::multimap<Entry::Name, Entry>;

int main()
{

    std::string filename = "out100000.csv";

    // This is not working, perhaps because of some WSL problem
//    if (std::filesystem::path(filename).extension() != ".csv")
//        throw std::runtime_error("Invalid format of file!");

    if (get_file_ext(filename) != "csv")
    {
        std::cerr << "Invalid file format!\n";
        return 1;
    }

    Data data = data_from_csv(filename);

    std::vector<std::size_t> sizes =
    {100, 200, 400, 600, 800, 1000, 1500, 2000, 3000, 4000, 5500, 8000, 11000, 15000, 20000, 30000, 40000, 50000, 70000, 100000};

    std::vector<std::string> names = {"Lindsay Sandoval", "Annis Pollard", "Keiko Kelly", "Sylvie Nieves",
                                     "Ed Porter", "Vernetta Diaz", "Abram Mcintyre", "Soraya Sharpe",
                                      "Clorinda Briggs", "Lorina Blackburn", "Milly Sampson", "Jarrod Bishop"};

    std::vector<my_tuple> statistics;

    std::vector<std::uint64_t> time;
    std::uint64_t time_mean;

    std::string act;

    //mmap_name_entry mmap_Entry = data_to_map(data);

    for (std::size_t size : sizes)
    {
        std::cout << "-----Size: " << size << "------\n";

        act = "Linear search";
        std::cout << "Running " << act << "... " << std::flush;
        for (const std::string& name : names)
        {
           time.push_back(get_time_lin_search(name, data, size));
        }
        time_mean = static_cast<std::uint64_t>(std::accumulate(time.begin(), time.end(), 0.0/time.size()));
        statistics.emplace_back(size, act, time_mean);
        time.clear();
        std::cout << "Done.\n";


        act = "Binary search (presorted)";
        std::cout << "Running " << act << "... " << std::flush;
        for (const std::string& name : names)
        {
           time.push_back(get_time_bin_search(name, data, size));
        }
        time_mean = static_cast<std::uint64_t>(std::accumulate(time.begin(), time.end(), 0.0/time.size()));
        statistics.emplace_back(size, act, time_mean);
        time.clear();
        std::cout << "Done.\n";


        act = "Binary search (with sort)";
        std::cout << "Running " << act << "... " << std::flush;
        for (const std::string& name : names)
        {
           time.push_back(get_time_bin_search(name, data, size, true));
        }
        time_mean = static_cast<std::uint64_t>(std::accumulate(time.begin(), time.end(), 0.0/time.size()));
        statistics.emplace_back(size, act, time_mean);
        time.clear();
        std::cout << "Done.\n";

        act = "Multimap search";
        std::cout << "Running " << act << "... " << std::flush;
        for (const std::string& name : names)
        {
           time.push_back(get_time_mmap_search(name, data, size));
        }
        time_mean = static_cast<std::uint64_t>(std::accumulate(time.begin(), time.end(), 0.0/time.size()));
        statistics.emplace_back(size, act, time_mean);
        time.clear();
        std::cout << "Done.\n";

    } // end "for" on sizes

    times_to_csv("times.csv", statistics);
}
