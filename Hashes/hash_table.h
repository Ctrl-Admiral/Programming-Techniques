/*
 * @file
 * Содержит реализацию хэш-таблицы
 */

#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <vector>
#include <utility>


namespace study
{

template <typename Key, typename T>
class HashTable
{
public:
    using hash_function = std::function<std::size_t(const Key)>;

    HashTable(hash_function f)
        : hasher_(std::move(f)),
          table_(std::vector<Bucket>(11))
    {}

    HashTable(const HashTable&) = default;
    HashTable& operator=(const HashTable&) = default;
    HashTable(HashTable&&) noexcept = default;
    HashTable& operator=(HashTable&&) noexcept = default;

    static inline std::list<T> empty_list{};

    /**
     * @brief Ищет в таблице все элементы, соответствующие ключу
     * @param[in] key ключ, по которому осуществляется поиск
     * @return std::list из значений типа T, которые хранятся по ключу key
     */
    const std::list<T>& equal_range(const Key& key) const
    {
        const Bucket& cur_bucket = table_[index(key)];

        for (InBucket elem : cur_bucket)
            if (elem.key() == key)
                return elem.values();

        return empty_list;
    }

    /**
     * @brief Вставляет элемент с ключом и значением в хэш-таблицу
     * @param[in] key
     * @param[in] value
     */
    void emplace(const Key& key, T value)
    {
        std::list<T> values {value};
        emplace(key, values);
    }

    /**
     * @brief Вставляет элемент с ключом и списком его значений в хэш-таблицу
     * @param[in] key
     * @param[in] values
     */
    void emplace(const Key& key, std::list<T> values)
    {
        Bucket& bucket = table_[index(key)];
        for (InBucket& elem : bucket)
        {
            if(elem.key() == key)
            {
                for (T& value : values)
                    elem.values().emplace_front(std::move(value));
                return;
            }
        }

        // Проверка на заполненность корзины
        if(bucket.size() != bucket_size)
        {
            if (bucket.empty())
                ++empty_bucket_count;
            bucket.emplace_front(key, std::move(values));
        }
        else
        {
            rehash();
            emplace(key, std::move(values));
        }
    }

private:

    /**
     * @brief Класс, члены которого -- элементы в корзине. Содержит ключ key и соответствующую ему структуру.
     */
    class InBucket
    {
    public:
        InBucket(Key key, std::list<T> values)
            : key_(std::move(key))
            , values_(std::move(values))
        {}

        const Key& key() const { return key_; }
        Key& key() { return key_; }
        const std::list<T>& values() const { return values_ ; }
        std::list<T>& values() { return values_ ; }

    private:
        Key key_;
        std::list<T> values_;
    };

    /**
     * @brief Ищет индекс элемента в хэш-таблице
     * @param[in] key
     * @return индекс элемента в хэш-таблице
     */
    std::size_t index(const Key& key) const
    {
        return hasher_(key) % table_.size();
    }

    /**
     * @brief Перехэширует таблицу, если было превышено максимальное количество
     * пустых корзин или превышен максимальный размер одной из корзин
     */
    void rehash()
    {
        std::size_t new_table_size = table_.size();
        std::size_t new_bucket_size = bucket_size;
        if (empty_bucket_count * 7 > table_.size())
        {
            new_table_size = table_.size() * 2;
        }
        else
        {
            ++new_bucket_size;
        }

        //Создание новой хэш-таблицы
        HashTable new_hashtable(hasher_);
        new_hashtable.table_.resize(new_table_size);
        new_hashtable.bucket_size = new_bucket_size;

        for(Bucket& bucket : table_)
            for (InBucket elem : bucket)
                new_hashtable.emplace(std::move(elem.key()), std::move(elem.values()));
        std::swap(*this, new_hashtable);
    }

    using Bucket = std::list<InBucket>;

    hash_function hasher_;
    std::vector<Bucket> table_;
    std::size_t empty_bucket_count = 0;
    std::size_t bucket_size = 3;    //Для ограничения числа элементов в корзине


};

} //namespace study
