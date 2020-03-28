/**
 * @file
 * @brief Файл, содержащий реализацию сортировки вставками (insertions sort).
 * @details Some details
 */

#pragma once

#include <cstdlib>
#include <iterator>

namespace study
{
    /**
     * Реализует сортировку вставками диапазона элементов
     * @tparam Iterator
     * @tparam Compare
     * @param[in,out] begin, end итераторы, указывающие на диапазон, который нужно отсортировать
     * @param[in] cmp компаратор, проверяющий, должен ли его первый аргумент стоять правее второго
     */
    template< typename BiDirIterator, typename Compare>
    void insertions_sort(BiDirIterator first, BiDirIterator last, Compare cmp)
    {
        if (first > last)
            throw std::runtime_error("First iterator is bigger than last");

         BiDirIterator tmp = std::next(first);   // flag there is place of elem to be sorted
         BiDirIterator cur_elem;                 // pointer to currently sortable elem

         for ( ; tmp != last; ++tmp)
         {
             cur_elem = tmp;
             while (cur_elem != first && cmp(*cur_elem, *std::prev(cur_elem)))
             {
                 std::iter_swap(cur_elem, --cur_elem);
             }
          }
    }

    /**
     * Реализует сортировку вставками диапазона элементов
     * @tparam Iterator
     * @param[in,out] begin, end итераторы, указывающие на диапазон, который нужно отсортировать
     */
    template< typename BiDirIterator>
    void insertions_sort(BiDirIterator first, BiDirIterator last)
    {
        insertions_sort(first, last, std::less< typename std::iterator_traits<BiDirIterator>::value_type >());
    }
}
