/**
 * @file
 * Содержащит реализацию хэш-функций
 */

#include <string>

namespace study
{
std::size_t simple_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (char letter : str)
        hash += static_cast<uint32_t>(letter);
    return hash;
}

std::size_t rot5_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (char let : str)
    {
        hash += static_cast<std::uint32_t>(let);
        hash -= (hash << 5) | (hash >> 13);
    }
    return hash;
}

std::size_t rot13_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (char c : str)
    {
        hash += static_cast<std::uint8_t>(c);
        hash -= (hash << 13) | (hash >> 19);
    }
    return hash;
}

std::size_t elf_hash(const std::string& str)
{
    std::size_t hash = 0, x;

    for (char c : str)
    {
        hash = (hash << 4) + static_cast<std::size_t>(c);
        if ((x = (hash & 0xF0000000)))
            hash ^= x >> 24;
        hash &= ~x;
    }
    return hash;
}

std::size_t better_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (std::size_t i = 0; i < str.length(); ++i)
        hash += (i * static_cast<std::size_t>(str[i]) - hash * 11) & 0xFFFFFFFF;
    return hash;
}

} // namespace study

