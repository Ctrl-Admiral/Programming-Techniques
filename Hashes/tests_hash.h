#include "entry.h"
#include <chrono>
#include <functional>
#include <vector>

using Data = std::vector<Entry>;
using my_tuple = std::tuple<std::size_t, std::string, std::uint64_t>;
using my_tuple2 = std::tuple<std::size_t, std::string, double>;
using my_hasher = std::function<std::size_t(Entry::Name)>;

/**
 * @brief search_hash_timing
 * @param data
 * @param size
 * @param keys
 * @param hasher
 * @return число std::uint64_t -- время, затраченное на поиск в хэш-таблице
 */
std::uint64_t search_hash_timing(const Data& data,
                                 const std::size_t& size,
                                 const std::vector<Entry::Name>& keys,
                                 const my_hasher& hasher);

/**
 * @brief search_hash_timing_all
 * @param data
 * @param sizes
 * @param keys
 * @return
 */
std::vector<my_tuple> search_hash_timing_all(const Data& data,
                                             const std::vector<std::size_t>& sizes,
                                             const std::vector<Entry::Name>& keys);

/**
 * @brief collisions_hash_counting
 * @param data
 * @param hasher
 * @param size
 * @return
 */
double collisions_hash_counting(const Data& data, const my_hasher& hasher, std::size_t size);

/**
 * @brief collisions_hash_count_all
 * @param data
 * @param sizes
 * @return
 */
std::vector<my_tuple2> collisions_hash_count_all(const Data& data, std::vector<std::size_t> sizes);
