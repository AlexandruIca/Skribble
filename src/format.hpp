#ifndef FORMAT_HPP
#define FORMAT_HPP
#pragma once

#include <QString>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace sk {

template<int N, typename... Ts>
auto format(char const (&fmt)[N], Ts&&... args) -> std::string
{
    static thread_local std::stringstream ss;
    QString result{ fmt };

    ((result = result.arg(
          QString::fromStdString(((ss << std::forward<Ts>(args)), ss.str()))),
      ss.str("")),
     ...);

    return result.toStdString();
}

template<int N, typename... Ts>
auto printTo(std::ostream& os, char const (&fmt)[N], Ts&&... args) -> void
{
    os << format(fmt, std::forward<Ts>(args)...);
}

template<int N, typename... Ts>
auto printlnTo(std::ostream& os, char const (&fmt)[N], Ts&&... args) -> void
{
    os << format(fmt, std::forward<Ts>(args)...) << std::endl;
}

template<int N, typename... Ts>
auto print(char const (&fmt)[N], Ts&&... args) -> void
{
    printTo(std::cout, fmt, std::forward<Ts>(args)...);
}

template<int N, typename... Ts>
auto println(char const (&fmt)[N], Ts&&... args) -> void
{
    printlnTo(std::cout, fmt, std::forward<Ts>(args)...);
}

} // namespace sk

#endif // !FORMAT_HPP
