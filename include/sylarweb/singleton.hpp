#ifndef __SYLAR_SINGLETON_GUARD__
#define __SYLAR_SINGLETON_GUARD__

#include <memory>

namespace Cot {

template <typename T, typename Tag, typename TagIdx>
auto getInstanceX()
    -> T&
{
    static auto sEntity = T();
    return sEntity;
}

template <typename T, typename Tag, typename TagIdx>
auto getInstanceSptr()
    -> std::shared_ptr<T>
{
    static auto sEntitySptr = std::shared_ptr<T>(new T);
    return sEntitySptr;
}

/**
 * @brief 单例模式封装类
 * @details T 类型
 *          X 为了创造多个实例对应的Tag
 *          N 同一个Tag创造多个实例索引
 */
template <class T, class X = void, int N = 0>
class Singleton {
public:
    /**
     * @brief 返回单例裸指针
     */
    static T* getInstance()
    {
        static T v;
        return &v;
        //return &GetInstanceX<T, X, N>();
    }
};

/**
 * @brief 单例模式智能指针封装类
 * @details T 类型
 *          X 为了创造多个实例对应的Tag
 *          N 同一个Tag创造多个实例索引
 */
template <class T, class X = void, int N = 0>
class SingletonPtr {
public:
    /**
     * @brief 返回单例智能指针
     * @todo 也许这里返回值可以改为std::weak_ptr,但两者开销差不多，也无所谓吧
     */
    static std::shared_ptr<T> getInstance()
    {
        static std::shared_ptr<T> v(new T);
        return v;
        //return GetInstancePtr<T, X, N>();
    }
};

} //namespace Sylar

#endif