/**
  * @file
  * @brief Заголовочный файл, содержащий объявления вспомогательных функций
  */

#pragma once

#include "entry.h"
#include <functional>
#include <map>
#include <vector>

using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;
using mmap_name_entry = std::multimap<Entry::Name, Entry>;

/**
 * @brief Считывает данные из данного csv-файла и записывает в вектор, состоящий из объектов класса Entry
 * @param[in] filename имя/путь к файлу с данными для чтения
 * @param[in] sep использующийся в csv-файле разделитель
 * @return вектор из объектов класса Entry, определенного в "entry.h"
 */
Data data_from_csv(const std::string& filename, char sep=',');

/**
 * @brief Записывает данные типа std::vector<Entry> в формате csv в файл
 * @param[out] filename файл, в который будут записаны данные
 * @param[in] data данные для записи в файл
 * @param[in] sep разделитель для csv-файла
 */
void data_to_csv(const std::string& filename, const Data& data, char sep=',');

/**
 * @brief Измеряет время работы линейного поиска
 * @param[in] key ключ, по которому производится поиск
 * @param[in] data данные, в которых будет производиться поиск
 * @param[in] size количество строк, в которых будет производиться поиск
 * @return число типа std::uint64_t -- время, затраченное на поиск
 */
std::uint64_t get_time_lin_search(const Entry::Name& key, const Data& data, const std::size_t& size);

/**
 * @brief Измеряет время работы бинарного поиска
 * @param[in] key ключ, по которому производится поиск
 * @param[in] data данные, в которых будет производиться поиск
 * @param[in] size количество строк, в которых будет производиться поиск
 * @param[in] time_before_sort начинать отсчет времени до сортировки (true) или после (false)
 * @return число типа std::uint64_t -- время, затраченное на поиск или сортировку + поиск
 */
std::uint64_t get_time_bin_search(const Entry::Name& key, const Data& data,
                                  const std::size_t& size, bool time_before_sort=false);

/**
 * Копирует часть данных и возвращает в виде вектора объектов класса Entry
 * @param[in] data входной набор данных
 * @param[in] size размер той части, которую нужно "отщипнуть" и скопировать
 * @return набор считанных данных в формате std::vector<Entry>
 */
Data get_slice_of_data(const Data& data, const std::size_t& size);

/**
 * Записывает данные о времени работы сортировок в файл в формате csv
 * @param[out] filename файл для выходных данных
 * @param[in] statistics вектор, содержащий tuple с данными о времени работы сортировок
 * @param[in] sep разделитель для формата csv
 */
void times_to_csv(const std::string& filename, const std::vector<my_tuple>& statistics, char sep=',');

/**
 * @brief Получает расширение файла: в строке-имени файла ищет с конца точку и отрезает все после нее
 * @param[in] filename имя файла
 * @return расширение файла, если точка -- не последний символ в имени и пустую строку в противном случае
 */
std::string get_file_ext(const std::string& filename);

/**
 * Записывает данные в формате Data в контейнер multimap
 * @param[in] data считываемые данные
 * @return контейнер multimap<Entry::Name, Entry>
 */
mmap_name_entry data_to_map(const Data& data);

/**
 * Подсчитывает время поиска в контейнере multimap
 * @param[in] key ключ, по которому производится поиск
 * @param[in] data данные, в которых будет производиться поиск
 * @param[in] size количество строк, в которых будет производиться поиск
 * @return число типа std::uint64_t -- время, затраченное на поиск
 */
std::uint64_t get_time_mmap_search(const Entry::Name& key, const Data& data,
                                   const std::size_t& size);
