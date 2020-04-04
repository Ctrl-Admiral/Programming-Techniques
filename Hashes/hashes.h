#include <string>
#pragma once

namespace study
{
/**
 * @brief Реализует функцию тривиального хэширования
 * @param[in] str хэшируемая строка
 * @return хэш типа std::size_t
 */
std::size_t simple_hash(const std::string& str);
/**
 * @brief Реализует хэш-функцию rot5
 * @param[in] str хэшируемая строка
 * @return хэш типа std::size_t
 */
std::size_t rot5_hash(const std::string& str);
/**
 * @brief Реализует хэш-функцию rot13
 * @param[in] str хэшируемая строка
 * @return хэш типа std::size_t
 */
std::size_t rot13_hash(const std::string& str);
/**
 * @brief Реализует хэш-функцию Elf
 * @param[in] str хэшируемая строка
 * @return хэш типа std::size_t
 */
std::size_t elf_hash(const std::string& str);
/**
 * @brief Реализует улучшенную хэш-функцию с меньшим числом коллизий
 * @param[in] str хэшируемая строка
 * @return хэш типа std::size_t
 */
std::size_t better_hash(const std::string& str);
}
