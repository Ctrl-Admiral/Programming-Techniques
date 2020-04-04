/**
  * @file
  * @brief Заголовочный файл, содержащий реализацию линейного поиска
  */

#pragma once

#include <iterator>
#include <vector>

namespace study
{
    /**
     * Линейный поиск всех элементов, удовлятворяющих ключу
     * @tparam Iterator
     * @tparam Key тип ключа
     * @tparam Comparator бинарный предикат согласно C++ named requirements
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key ключ, элемент, по которому производится поиск
     * @param[in] cmp проверяет, эквивалентен ли проверяемый объект ключу
     * @return std::vector из итераторов, указывающих на начало элементов диапазона, эквивалентных ключу
     */
    template<typename Iterator, typename Key, typename Сomparator>
    std::vector<Iterator> linear_search(Iterator begin, Iterator end, const Key& key, Сomparator cmp)
    {
        std::vector<Iterator> res;
        for (; begin < end; ++begin)
        {
            if (cmp(*begin, key)) res.push_back(begin);
        }
        return res;
    }

    /**
     * Линейный поиск всех элементов, удовлятворяющих ключу
     * @tparam Iterator
     * @tparam Key тип ключа
     * @param[in] begin, end итераторы, указывающие на начало и конец диапазона поиска
     * @param[in] key элемент, по которому производится поиск
     * @return std::vector из итераторов, указывающих на начало элементов диапазона, эквивалентных ключу
     */
    template<typename Iterator, typename Key>
    std::vector<Iterator> linear_search(Iterator begin, Iterator end, const Key& key)
    {
        return linear_search(begin, end, key, std::equal_to<typename std::iterator_traits<Iterator>::value_type>());
    }
}

