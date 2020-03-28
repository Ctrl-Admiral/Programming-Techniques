/**
  * @file
  * @brief Заголовочный файл, содержащий определение класса Entry
  */

#pragma once

#include<iostream>
#include <utility> // for std::move - flag that object may be "moved from"

/**
 * @class Entry
 * @brief Класс для описания членов олимпийской команды
 */
class Entry
{
public:
    using Name    = std::string;
    using Age     = int;
    using Height  = int;
    using Weight  = int;
    using Sport   = std::string;

    Entry() = delete;

    Entry(Name name, Age age, Height height,
          Weight weight, Sport sport)
        : name_(std::move(name))
        , age_(age)
        , height_(height)
        , weight_(weight)
        , sport_(std::move(sport))
    {}

    Name    getName()   const { return name_; }
    Age     getAge()    const { return age_; }
    Height  getHeight() const { return height_; }
    Weight  getWeight() const { return weight_; }
    Sport   getSport()  const { return sport_; }

    /**
     * @brief Выводит члены класса в заданный поток вывода в формате csv с заданным разделителем
     * @param[out] stream поток вывода
     * @param[in] sep разделитель для csv
     */
    void to_csv(std::ostream& fout, char sep=',');

    friend bool operator==(const Entry& lhs, const Entry& rhs);
    friend bool operator!=(const Entry& lhs, const Entry& rhs);
    friend bool operator<(const Entry& lhs, const Entry& rhs);
    friend bool operator>(const Entry& lhs, const Entry& rhs);
    friend bool operator<=(const Entry& lhs, const Entry& rhs);
    friend bool operator>=(const Entry& lhs, const Entry& rhs);
    friend std::ostream& operator<<(std::ostream& stream, const Entry& entry);

private:
    Name name_;
    Age age_;
    Height height_;
    Weight weight_;
    Sport sport_;
};

/**
 * @brief Считывает строку из csv и создает объект класса Entry
 * @param[in] line строка csv-файла
 * @param[in] sep разделитель, использующийся в csv-файле
 * @return объект класса Entry
 */
Entry get_line_from_csv(const std::string& line, char sep=',');

