#include "functions.h"
#include "entry.h"
#include "hashes.h"
#include "tests_hash.h"
#include <iostream>
#include <vector>


using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;
using my_tuple2 = std::tuple<std::size_t, std::string, double>;

int main()
{
    std::string filename = "out100000.csv";

    if (get_file_ext(filename) != "csv")
    {
        std::cerr << "Invalid file format!\n";
        return 1;
    }

    Data data = data_from_csv(filename);

    std::vector<std::size_t> sizes =
    {100, 200, 400, 600, 800, 1000, 1500, 2000, 3000, 4000, 5500, 8000, 11000, 15000, 20000, 30000, 40000, 50000, 70000, 100000};

    std::vector<Entry::Name> names = {"Lindsay Sandoval", "Annis Pollard", "Keiko Kelly", "Sylvie Nieves",
                                     "Ed Porter", "Vernetta Diaz", "Abram Mcintyre", "Soraya Sharpe",
                                      "Clorinda Briggs", "Lorina Blackburn", "Milly Sampson", "Jarrod Bishop"};

    std::cout << "Start timing of searches..." << '\n';
    std::vector<my_tuple> time_statistics = search_hash_timing_all(data, sizes, names);

    statistics_to_csv<std::vector<my_tuple>>("hash_timings.csv", time_statistics);

    std::cout << "\nStart timing of counting collisions..." << '\n';

    std::vector<my_tuple2> coll_statistics = collisions_hash_count_all(data, sizes);
    statistics_to_csv<std::vector<my_tuple2>>("hash_collisions.csv", coll_statistics);


}
