#pragma once

#include "entry.h"
#include <fstream>
#include <string>
#include <vector>


using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;

/**
 * @brief Считывает данные из данного csv-файла и записывает в вектор, состоящий из объектов класса Entry
 * @param[in] filename имя/путь к файлу с данными для чтения
 * @param[in] sep использующийся в csv-файле разделитель
 * @return вектор из объектов класса Entry, определенного в "entry.h"
 */
Data data_from_csv(const std::string& filename, char sep=',');

/**
 * @brief Получает расширение файла: в строке-имени файла ищет с конца точку и отрезает все после нее
 * @param[in] filename имя файла
 * @return расширение файла, если точка -- не последний символ в имени и пустую строку в противном случае
 */
std::string get_file_ext(const std::string& filename);

/**
 * Записывает данные в файл в формате csv
 * @param[out] filename файл для выходных данных
 * @param[in] statistics вектор, содержащий статистическую информацию
 * @param[in] sep разделитель для формата csv
 */
template<typename MyVector>
void statistics_to_csv(const std::string& filename, const MyVector& statistics, char sep=',')
{
    std::ofstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Cannot open output csv-file");

    for (const auto& elem : statistics)
    {
        const auto& [ size, sort, t ] = elem;
        f << size << sep << sort << sep << t << '\n';
    }
    f.close();
}
