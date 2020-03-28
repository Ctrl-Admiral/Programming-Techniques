/**
 * @file
 * @brief Файл, содержащий реализацию быстрой сортировки(quick sort).
 * @details Some loooong details.
 */

#pragma once

#include <cstdlib>
#include <iterator>

namespace study
{
    /**
     * Реализует быструю сортировку диапазона элементов
     * @tparam Iterator
     * @tparam Compare
     * @param[in,out] begin, end итераторы, указывающие на диапазон, который нужно отсортировать
     * @param[in] cmp компаратор, проверяющий, должен ли его первый аргумент стоять правее второго
     */
     template< typename Iterator, typename Compare >
     void q_sort(Iterator first, Iterator last, Compare cmp)
     {
       if (first > last) throw std::runtime_error("Begin iterator is bigger than end.");\

       if(first != last)
       {
         Iterator left  = std::next(first);
         Iterator right = last;

         while(left != right)
         {
           if(cmp(*left, *first))
           {
              ++left;   // find elem from left bigger than first
           }
           else
           {
              while ((left != --right) && cmp(*first, *right)); // find elem from right less than first
              std::iter_swap(left, right);  // and swap it with left
           }
         }
         --left;    // get prev left to make it before right
         std::iter_swap(first, left);

         // recursion for 2 parts of sequency
         q_sort(first, left, cmp);
         q_sort(right, last, cmp);
       }
     }

    /**
    * Реализует быструю сортировку диапазона элементов
    * @tparam Iterator
    * @param[in,out] begin, end итераторы, указывающие на диапазон, который нужно отсортировать
    */
    template<typename Iterator>
    void q_sort(Iterator first, Iterator last)
    {
        q_sort(first, last, std::less< typename std::iterator_traits<Iterator>::value_type >());
    }
}
