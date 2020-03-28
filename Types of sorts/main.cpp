/**
 * @mainpage
 * Проект с реализацией сортировок:
 * пирамидальной, простыми вставками и быстрой, а также
 * замером времени их работы.
 * Данные для сортировки содержатся в csv-файле,
 * сгенерированном вне данного проекта (out100000.csv).
 * Время работы каждой сортировки на данных каждого объема
 * записывается в файл times.csv.
 */

#include "entry.h"
#include "functions.h"
#include "sorts.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;

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
//    data_to_csv("test.csv", data, ',');

    std::vector<std::size_t> sizes =
    {100, 200, 400, 600, 800, 1000, 1500, 2000, 3000, 4000, 5500, 8000, 11000, 15000, 20000, 30000, 40000, 50000, 70000, 100000};

    std::vector<std::size_t> test_sizes = {100, 10000, 80000};

    std::vector<my_tuple> statistics;
    std::uint64_t time;

    for (std::size_t size : test_sizes)
    {
        std::cout << "-----Size: " << size << "------\n";

        std::string str_sort = "QuickSort";
        std::cout << "Running " << str_sort << "..." << std::flush;
        time = get_time_sort(study::q_sort<Data::iterator>, data, size);
        statistics.emplace_back(std::make_tuple(size, str_sort, time));
        std::cout << "Done.\n";

        str_sort = "InsertionSort";
        std::cout << "Running " << str_sort << "..." << std::flush;
        time = get_time_sort(study::insertions_sort<Data::iterator>, data, size);
        statistics.emplace_back(std::make_tuple(size, str_sort, time));
        std::cout << "Done.\n";

        str_sort = "HeapSort";
        std::cout << "Running " << str_sort << "..." << std::flush;
        time = get_time_sort(study::heap_sort<Data::iterator>, data, size);
        statistics.emplace_back(std::make_tuple(size, str_sort, time));
        std::cout << "Done.\n";
    }

    //times_to_csv("times.csv", statistics);

}
