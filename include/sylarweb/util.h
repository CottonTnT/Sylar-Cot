#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <iostream>

namespace Sylar {

auto getElapseMs()
    -> uint64_t;

auto getThreadId()
    -> pid_t;
auto getFiberId()
    -> pid_t;

/**
* @brief 获取T类型的类型字符串
*/
// template <class T>
// const char *TypeToName() {
//     static const char *s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
//     return s_name;
// }
template <typename T>
auto typeToName()
    -> std::string_view
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    static auto str = std::string(__PRETTY_FUNCTION__);
    auto pos        = str.find("T = ");
    pos += 4;
    auto pos2 = str.find_first_of(";]", pos);
    return std::string_view(str.data() + pos, pos2 - pos);
}

} //namespace Cot
#endif