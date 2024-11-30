#ifndef UTIL_H
#define UTIL_H

#include <memory>

namespace Sylar {

template <typename Derived>
class SelfSptr {
public:
    using Sptr = std::shared_ptr<Derived>;
};

template <typename T>
using DefineSptr = std::shared_ptr<T>;

auto getElapseMs()
    -> uint64_t;

auto getThreadId()
    -> pid_t;
auto getFiberId()
    -> pid_t;
} //namespace Cot
#endif