/**
 * @file
 * @brief Файл, содержащий реализацию пирамидальной сортировки (heap sort).
 * @details Куча строится по следующему принципу: сыновьями элемента a[i] являются a[2i+1] и a[2i+2]
 * Предок всегда больше своих детей.
 */

#pragma once

#include <iterator>

/**
 * @namespace study
 * @brief Пространство имен, содержащее все сортировки
 */
namespace study
{
    /**
     * Вспомогательная функция "просеивания вниз" для реализации пирамидальной сортировки
     * @tparam Iterator
     * @tparam Compare
     * @param[in,out] begin итератор, указывающий на начало диапазона
     * @param[in,out] cur_it итератор, указывающий на "просеиваемый" элемент
     * @param[in] heap_size размер "кучи"
     * @param[in] cmp компаратор, проверяющий, должен ли его первый аргумент стоять правее второго
     */
    template<typename Iterator, typename Compare>
    void sift_down(Iterator begin, Iterator cur_it,
                   typename std::iterator_traits<Iterator>::difference_type heap_size,
                   Compare cmp)
    {
        using diff_t = typename std::iterator_traits<Iterator>::difference_type;
        diff_t cur_elem_dist = std::distance(begin, cur_it);
        Iterator cur_elem_it;
        //std::cout << "dist from begin to cur_it = " << elem_to_sift_dist << '\n';
        //Iterator elem_to_sift_it = std::next(begin, elem_to_sift_dist);

        while (2 * cur_elem_dist + 1 < heap_size)
        {
            diff_t left_dist     = 2 * cur_elem_dist + 1; // distance to left son from begin
            diff_t right_dist    = 2 * cur_elem_dist + 2; // distance to right son from begin

            Iterator left_son = std::next(begin, left_dist);
            Iterator right_son = std::next(begin, right_dist);
            Iterator bigger_son = left_son;


            if ( right_dist < heap_size && cmp(*left_son, *right_son) )
                bigger_son = right_son;

            cur_elem_it = std::next(begin, cur_elem_dist);

            if(!cmp(*cur_elem_it, *bigger_son))
                break;

            std::iter_swap(cur_elem_it, bigger_son);

            cur_elem_dist = std::distance(begin, bigger_son);
        }
    }

    /**
     * Вспомогательная функция создания кучи для реализации пирамидальной сортировки
     * @param[in,out] begin итератор, указывающий на начало диапазона
     * @param[in] heap_size размер "кучи"
     * @param[in] cmp компаратор, проверяющий, должен ли его первый аргумент стоять правее второго
     */
    template<typename Iterator, typename Compare>
    void build_heap(Iterator begin, typename std::iterator_traits<Iterator>::difference_type heap_size, Compare cmp)
    {
        for (Iterator i = std::next(begin, heap_size / 2); i != std::prev(begin); --i)
            sift_down(begin, i, heap_size, cmp);
    }

    /**
     * Реализует пирамидальную сортировку диапазона элементов
     * @tparam Iterator
     * @tparam Compare
     * @param[in,out] begin, end итераторы, указывающие на диапазон, который нужно отсортировать
     * @param[in] cmp компаратор, проверяющий, должен ли его первый аргумент стоять правее второго
     */
    template<typename Iterator, typename Compare>
    void heap_sort(Iterator begin, Iterator end, Compare cmp)
    {
        if (begin > end)
            throw std::runtime_error("First iterator is bigger than last");

        using diff_t = typename std::iterator_traits<Iterator>::difference_type;
        diff_t size = std::distance(begin, end);
        //std::cout << size << '\n';
        build_heap(begin, size, cmp);
        //std::cout << "heap was built\n";

        diff_t heap_size = size;
        for (diff_t i = 0; i < size; ++i)
        {
            std::iter_swap(begin, std::prev(end, i + 1));
            --heap_size;
            //std::cout << "size is " << size << '\n';
            sift_down(begin, begin, heap_size, cmp);
        }
    }

    /**
     * Реализует пирамидальную сортировку диапазона элементов
     * @tparam Iterator
     * @param[in,out] begin, end итераторы, указывающие на диапазон, который нужно отсортировать
     */
    template< typename Iterator>
    void heap_sort(Iterator first, Iterator last)
    {
        heap_sort(first, last, std::less< typename std::iterator_traits<Iterator>::value_type >());
    }
}
