#ifndef __SYLAR_NONCOPYABLE_GUARD__
#define __SYLAR_NONCOPYABLE_GUARD__

namespace Cot {

class NonCopyable {
protected: //防止该类在类外实例化
    /**
     * @brief 默认构造函数
     */
    NonCopyable()  = default;
    ~NonCopyable() = default;

public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

class NonCopyOrMoveable {
protected: //防止该类在类外实例化
    NonCopyOrMoveable() = default;

    ~NonCopyOrMoveable() = default;

public:
    NonCopyOrMoveable(NonCopyOrMoveable&&) = delete;
};

}

#endif