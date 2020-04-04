/**
  * @file
  * @brief Содержит перегрузки операторов и определения функций считывания данных из csv-файла
  * и записи в csv-файл.
  * @details Файл содержит перегрузки операторов сравнения для членов класса Entry, а также
  * определения функций get_line_from_csv и Entry::to_csv.
  */

#include "entry.h"
#include <ostream>
#include <sstream>  // istringstream
#include <string>
#include <tuple>    // tie -- for creating tuples of lvalue references
#include <typeinfo>

bool operator<(const Entry& lhs, const Entry& rhs)
{
    // tie is for making temporary tuples of references
    return std::tie(lhs.sport_, lhs.name_, lhs.age_) <
           std::tie(rhs.sport_, rhs.name_, rhs.age_);
}

bool operator>(const Entry& lhs, const Entry& rhs)
{
    return lhs < rhs;
}

bool operator<=(const Entry& lhs, const Entry& rhs)
{
    return !(lhs > rhs);
}

bool operator>=(const Entry& lhs, const Entry& rhs)
{
    return !(lhs < rhs);
}

bool operator==(const Entry& lhs, const Entry& rhs)
{
    return std::tie(lhs.sport_, lhs.name_, lhs.age_) ==
           std::tie(rhs.sport_, rhs.name_, rhs.age_);
}

bool operator!=(const Entry& lhs, const Entry& rhs)
{
    return std::tie(lhs.sport_, lhs.name_, lhs.age_) !=
           std::tie(rhs.sport_, rhs.name_, rhs.age_);
}

std::ostream& operator<<(std::ostream& stream, const Entry& entry)
{
    stream << "-------Sportsman--------"
           << "\nName: \"" << entry.name_
           << "\"\nAge: \"" << entry.age_
           << "\"\nHeight: \"" << entry.height_
           << "\"\nWeight: \"" << entry.weight_
           << "\"\nSport: " << entry.sport_
           << '\n';
    return stream;
}

void Entry::to_csv(std::ostream& stream, char sep)
{
    stream << name_ << sep
         << age_ << sep
         << height_ << sep
         << weight_ << sep
         << sport_  << "\n";
}

Entry get_line_from_csv(const std::string& line, char sep)
{
    Entry::Name name;
    Entry::Age age;
    Entry::Height height;
    Entry::Weight weight;
    Entry::Sport sport;

    std::istringstream lin(line);
    std::getline(lin, name, sep);

    lin >> age;
    if (lin.get() != sep)
        throw std::runtime_error("Invalid input \"age\" from csv. Wrong separator or format of number.");

    lin >> height;
    if (lin.get() != sep)
        throw std::runtime_error("Invalid input \"height\" from csv. Wrong separator or format of number.");

    lin >> weight;
    if (lin.get() != sep)
        throw std::runtime_error("Invalid input \"weight\" from csv. Wrong separator or format of number");

    getline(lin, sport, sep);
    return Entry(name, age, height, weight, sport);
}
