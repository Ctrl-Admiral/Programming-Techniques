/**
  * @file
  * @brief Заголовочный файл, содержащий объявления вспомогательных функций
  */

#pragma once

#include "entry.h"
#include <functional>
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
 * @brief Записывает данные типа std::vector<Entry> в формате csv в файл
 * @param[out] filename файл, в который будут записаны данные
 * @param[in] data данные для записи в файл
 * @param[in] sep разделитель для csv-файла
 */
void data_to_csv(const std::string& filename, const Data& data, char sep=',');

/**
 * @brief Измеряет время работы функции сортровки из пространства имен Study
 * @param[in] sort_function используемая функция
 * @param[in] data данные, из которых будет скопирована и отсортирована часть нужного размера
 * @param[in] size количество строк, которые необходимо "отщипнуть" от полного набора и отсортировать
 * @return число типа std::uint64_t -- время работы функции
 */
std::uint64_t get_time_sort(const std::function<void(Data::iterator, Data::iterator)>& sort_function,
                          const Data& data, const std::size_t& size);

/**
 * @brief Копирует часть данных и возвращает в виде вектора объектов класса Entry
 * @param[in] data входной набор данных
 * @param[in] size размер той части, которую нужно "отщипнуть" и скопировать
 * @return набор считанных данных в формате std::vector<Entry>
 */
Data get_slice_of_data(const Data& data, const std::size_t& size);

/**
 * @brief Записывает данные о времени работы сортировок в файл в формате csv
 * @param[out] filename файл для выходных данных
 * @param[in] statistics вектор, содержащий tuple с данными о времени работы сортировок
 * @param[in] sep разделитель для формата csv
 */
void times_to_csv(const std::string& filename, const std::vector<my_tuple>& statistics, char sep=',');

std::string get_file_ext(const std::string& filename);
