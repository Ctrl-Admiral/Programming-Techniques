#include "entry.h"
#include "hash_table.h"
#include "hashes.h"
#include "tests_hash.h"
#include <algorithm>
#include <chrono>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <numeric> //for std::accumulate

using Data = std::vector<Entry>;
using Clock = std::chrono::high_resolution_clock;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;
using my_tuple2 = std::tuple<std::size_t, std::string, double>;
using my_hasher = std::function<std::size_t(Entry::Name)>;

std::uint64_t search_hash_timing(const Data& data,
                                 const std::size_t& size,
                                 const std::vector<Entry::Name>& keys,
                                 const my_hasher& hasher)
{
    using namespace std::chrono;
    std::uint64_t time;
    study::HashTable<Entry::Name, Entry> htable(hasher);

    Data::const_iterator data_size = std::next(data.begin(), static_cast<std::ptrdiff_t>(size));

    //Заполнение хэш-таблицы
    for (Data::const_iterator it = data.begin(); it != data_size; ++it)
        htable.emplace(it->getName(), *it);

    std::vector<std::uint64_t> times;
    std::uint64_t dif_t;

    for (const Entry::Name& key : keys)
    {
        time_point<Clock> start = Clock::now();
        const std::list<Entry>& ans = htable.equal_range(key);
        time_point<Clock> end = Clock::now();

        dif_t = static_cast<std::uint64_t>(duration_cast<nanoseconds>(end - start).count());
        times.emplace_back(dif_t);
    }
    time = static_cast<std::uint64_t>(std::accumulate(times.begin(), times.end(), 0.0/times.size()));
    return time;
}

std::vector<my_tuple> search_hash_timing_all(const Data& data,
                                             const std::vector<std::size_t>& sizes,
                                             const std::vector<Entry::Name>& keys)
{
    using namespace study;
    std::vector<std::string> names = {"Simple hash", "Rot5 hash", "Rot13 hash", "Elf hash", "Better hash"};
    std::vector<my_hasher> hashers = {simple_hash, rot5_hash, rot13_hash, elf_hash, better_hash};
    std::vector<std::pair<std::string, my_hasher>> names_and_hashers;

    for (std::size_t i = 0; i < hashers.size(); ++i)
    {
        names_and_hashers.emplace_back(std::make_pair(names[i], hashers[i]));
    }

    std::vector<my_tuple> time_statistics;
    std::uint64_t time;
    for (std::size_t size : sizes)
    {
        std::cout << "-----Size: " << size << "------\n";

        for (const std::pair<std::string, my_hasher>& elem : names_and_hashers)
        {
            std::string str_sort = elem.first;
            std::cout << "Running " << str_sort << "..." << std::flush;
            time = search_hash_timing(data, size, keys, elem.second);
            time_statistics.emplace_back(std::make_tuple(size, str_sort, time));
            std::cout << "Done.\n";
        }
    }
    return time_statistics;
}

double collisions_hash_counting(const Data& data, const my_hasher& hasher, std::size_t size)
{
    Data::const_iterator last = data.begin() + static_cast<std::ptrdiff_t>(size);
    Data part_data = Data(data.begin(), last);

    std::vector<std::string> vals_to_hash;
    std::size_t i = 0;
    for (const Entry& entry : part_data)
    {
        vals_to_hash.push_back(entry.getName() + std::to_string(i));
        ++i;
    }

    std::vector<std::string>::iterator end_it = std::unique(vals_to_hash.begin(), vals_to_hash.end());
    vals_to_hash.resize(static_cast<std::size_t>(std::distance(vals_to_hash.begin(), end_it)));
    std::map<size_t, size_t> hashes_and_count;
    std::size_t count = 0;
    for (std::string& s : vals_to_hash)
    {
        size_t hash = hasher(s);
        if (!hashes_and_count.count(hash))
        {
            hashes_and_count[hash] = 1;
        }
        else
        {
            ++hashes_and_count[hash];
            ++count;
        }
     }

    double ans = count * 100. / size;
    return ans;
}

std::vector<my_tuple2> collisions_hash_count_all(const Data& data, std::vector<std::size_t> sizes)
{
    using namespace study;
    std::vector<std::string> names = {"Simple hash", "Rot5 hash", "Rot13 hash", "Elf hash", "Better hash"};
    std::vector<my_hasher> hashers = {simple_hash, rot5_hash, rot13_hash, elf_hash, better_hash};
    std::vector<std::pair<std::string, my_hasher>> names_and_hashers;

    for (std::size_t i = 0; i < hashers.size(); ++i)
    {
        names_and_hashers.emplace_back(std::make_pair(names[i], hashers[i]));
    }

    std::vector<my_tuple2> col_statistics;
    double percent;

    for (std::size_t size : sizes)
    {
        std::cout << "-----Size: " << size << "------\n";

        for (const std::pair<std::string, my_hasher>& elem : names_and_hashers)
        {
            std::string str_sort = elem.first;
            std::cout << "Running " << str_sort << "..." << std::flush;
            percent = collisions_hash_counting(data, elem.second, size);
            col_statistics.emplace_back(std::make_tuple(size, str_sort, percent));
            std::cout << "Done.\n";
        }
    }
    return col_statistics;
}
