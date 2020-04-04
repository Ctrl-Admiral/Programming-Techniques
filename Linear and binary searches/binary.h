/**
  * @file
  * @brief Заголовочный файл, содержащий реализацию бинарного поиска
  */

#pragma once


#include <iterator>
#include <vector>

template<typename Iterator>
using elem_type = typename std::iterator_traits<Iterator>::value_type;
template<typename Iterator>
const elem_type<Iterator>& trivial_extractor(const elem_type<Iterator>& elem){ return elem; }

namespace study
{
    /**
     * Поиск в отсортированном первого элемента с начала, не меньшего, чем ключ
     * @tparam Iterator
     * @tparam Key тип ключа
     * @tparam Comparator бинарный предикат согласно C++ named requirements
     * @tparam KeyExtractor
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key элемент, по которому производится поиск
     * @param[in] cmp проверяет, должен ли его первый аргумент стоять левее второго в отсортированном диапазоне
     * @param[in] extractor функция, принимающая объект типа Iterator и возвращающая по нему значение типа Key,
     * которое будет сравниваться с ключом того же типа
     * @return объект типа Iterator, указывающий на найденный элемент
     */
    template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
    Iterator lower_bound(Iterator begin, Iterator end, const Key& key,
                         Comparator cmp, KeyExtractor extractor)
    {
        typename std::iterator_traits<Iterator>::difference_type dist = std::distance(begin, end);
        typename std::iterator_traits<Iterator>::difference_type mid;
        Iterator cur;
        while (dist != 0)
        {
            mid = dist / 2;
            cur = std::next(begin, mid);
            if (cmp(extractor(*cur), key))
            {
                // элемент меньше ключа -> переходим в правую половину диапазона
                begin = ++cur;
                dist -= mid + 1;
            }
            else
            {
                // элемент не меньше ключа -> переходим в левую половину диапазона:
                // начало не изменяется, но дистанция от begin до end сокращается вдвое
                dist = mid;
            }
        }
        return begin;
    }

    /**
     * Поиск в отсортированном первого элемента с конца, большего, чем ключ
     * @tparam Iterator
     * @tparam Key тип ключа
     * @tparam Comparator бинарный предикат согласно C++ named requirements
     * @tparam KeyExtractor
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key элемент, по которому производится поиск
     * @param[in] cmp проверяет, должен ли его первый аргумент стоять левее второго в отсортированном диапазоне
     * @param[in] extractor функция, принимающая объект типа Iterator и возвращающая по нему значение типа Key,
     * которое будет сравниваться с ключом того же типа
     * @return объект типа Iterator, указывающий на найденный элемент
     */
    template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
    Iterator upper_bound(Iterator begin, Iterator end, const Key& key,
                         Comparator cmp, KeyExtractor extractor)
    {
        typename std::iterator_traits<Iterator>::difference_type dist = std::distance(begin, end);
        typename std::iterator_traits<Iterator>::difference_type mid;
        Iterator cur;
        while (dist != 0)
        {
            mid = dist / 2;
            cur = std::next(begin, mid);

            if (!cmp(key, extractor(*cur)))
            {
                // ключ не меньше элемента -> переходим в правую половину диапазона
                begin = ++cur;
                dist -= mid + 1;
            }
            else
            {
                // элемент не меньше ключа -> переходим в левую половину диапазона:
                // начало не изменяется, но дистанция от begin до end сокращается вдвое
                dist = mid;
            }
        }
        return begin;
    }

    /**
     * Реализация бинарного поиска (бинарно ищется начало и конец диапазона эквивалентных ключу элементов)
     * @note Более корректное название функции equal_range по аналогии с функцией из библиотеки algorithm,
     * так как возвращает не первый найденный элемент, как стандартный бинарный поиск,
     * а диапазон всех эквивалентных ключу элементов
     * @tparam Iterator
     * @tparam Key тип ключа
     * @tparam Comparator бинарный предикат согласно C++ named requirements
     * @tparam KeyExtractor
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key элемент, по которому производится поиск
     * @param[in] cmp проверяет, должен ли его первый аргумент стоять левее второго в отсортированном диапазоне
     * @param[in] extractor функция, принимающая объект типа Iterator и возвращающая по нему значение типа Key,
     * которое будет сравниваться с ключом того же типа
     * @return пара итераторов, указывающих на начало и конец диапазона элементов, экваивалентных ключу
     */
    template<typename Iterator, typename Key, typename Сomparator, typename KeyExtractor>
    std::pair<Iterator, Iterator> binary_search(Iterator begin, Iterator end, const Key& key,
                                                             Сomparator cmp, KeyExtractor extractor)
    {
        Iterator first = lower_bound(begin, end, key, cmp, extractor);
        Iterator last = upper_bound(begin, end, key, cmp, extractor);
        return std::pair<Iterator, Iterator>(first, last);
    }

    /**
     * Реализация бинарного поиска (бинарно ищется начало и конец диапазона эквивалентных ключу элементов)
     * @note Более корректное название функции equal_range по аналогии с функцией из библиотеки algorithm,
     * так как возвращает не первый найденный элемент, как стандартный бинарный поиск,
     * а диапазон всех эквивалентных ключу элементов
     * @tparam Iterator
     * @tparam Key тип ключа
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key элемент, по которому производится поиск
     * @return пара итераторов, указывающих на начало и конец диапазона элементов, экваивалентных ключу
     */
    template<typename Iterator, typename Key>
    std::pair<Iterator, Iterator> binary_search(Iterator begin, Iterator end, const Key& key)
    {

        return binary_search(begin, end, key, std::less<elem_type<Iterator>>(), trivial_extractor<Iterator>);
    }

    /**
     * Реализация бинарного поиска (бинарно ищется начало и конец диапазона эквивалентных ключу элементов)
     * @note Более корректное название функции equal_range по аналогии с функцией из библиотеки algorithm,
     * так как возвращает не первый найденный элемент, как стандартный бинарный поиск,
     * а диапазон всех эквивалентных ключу элементов
     * @tparam Iterator
     * @tparam Key тип ключа
     * @tparam KeyExtractor
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key элемент, по которому производится поиск
     * @param[in] extractor функция, принимающая объект типа Iterator и возвращающая по нему значение типа Key,
     * которое будет сравниваться с ключом того же типа
     * @return пара итераторов, указывающих на начало и конец диапазона элементов, экваивалентных ключу
     */
    template<typename Iterator, typename Key, typename KeyExtractor>
    std::pair<Iterator, Iterator> binary_search(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor)
    {

        return binary_search(begin, end, key, std::less<elem_type<Iterator>>(), extractor);
    }
}
