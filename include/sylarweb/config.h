#ifndef COT_CONFIG_H
#define COT_CONFIG_H

#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <yaml-cpp/yaml.h>
#include "sylarweb/lexicalcast.hpp"
#include "sylarweb/mutex.h"
#include "sylarweb/util.h"

namespace Cot {
/**
 * @brief 配置项基类，虚基类，定义了配置项公有的成员和方法
 */
class ConfigVarBase {
public:
    using Sptr = std::shared_ptr<ConfigVarBase>;

    /**
     * @brief 构造函数
     * @param[in] name 配置参数名称[0-9a-z_.]
     * @param[in] description 配置参数描述
     */
    ConfigVarBase(std::string_view name,
                  std::string_view description = "")
        : m_name(name)
        , m_description(description)
    {
        //不区分大小写，统一按小写处理
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }

    virtual ~ConfigVarBase() = default;

    auto getName() const
        -> std::string_view { return m_name; }

    auto getDescription() const
        -> std::string_view { return m_description; }

    virtual auto toString()
        -> std::string = 0;

    virtual bool fromString(std::string_view val) = 0;

    /**
     * @brief 返回配置参数值的类型名称
     */
    virtual auto getTypeName()
        -> std::string const = 0;

protected:
    std::string m_name;        //配置项参数的名称
    std::string m_description; //配置参数的描述

private:
};

template <typename T,
          typename FromStr = LexicalCast<std::string, T>,
          typename ToStr   = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    using MutexType  = RWMutex;
    using Sptr       = std::shared_ptr<ConfigVar>;
    using OnChangeCb = std::function<void(const T& old_val, const T& new_value)>;

    ConfigVar(const std::string& name,
              const T& default_value,
              const std::string& description = "")
        : ConfigVarBase(name, description)
        , m_val(m_val)
    {
    }

    /**
     * @brief 将参数值转换成YAML String
     * @exception 当转换失败抛出异常
     */
    auto toString()
        -> std::string override
    {
        try
        {
            auto lk_guard = LockGuard<MutexType, ReadLockTag>(m_mutex);
            return ToStr()(m_val);
        } catch (std::exception& e)
        {
            // SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception "
            //                               << e.what() << " convert: " << TypeToName<T>() << " to string"
            //                               << " name=" << m_name;
        }
        return "";
    }

    /**
     * @brief 从YAML String 转成参数的值
     * @exception 当转换失败抛出异常
     */

    bool fromString(std::string_view val) override
    {
        try
        {
            auto lk_guard = LockGuard<MutexType, ReadLockTag>(m_mutex);
            return FromStr()(val);
        } catch (std::exception& e)
        {
            // SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::fromString exception "
            //                       << e.what() << " convert: string to " << TypeToName<T>()
            //                       << " name=" << m_name
            //                       << " - " << val;
        }
        return false;
    }

    auto getValue() const
        -> const T&
    {
        auto lk_guard = LockGuard<MutexType, ReadLockTag>(m_mutex);
        return m_val;
    }

    /**
     * @brief 设置当前参数的值
     * @details 如果参数的值有发生变化,则通知对应的注册回调函数
     */
    auto setValue(const T& val)
        -> void
    {
        //todo: reduce the race condition area
        {
            auto lk_guard = LockGuard<MutexType, ReadLockTag>(m_mutex);
            if (val == m_val)
            {
                return;
            }
            for (auto& i : m_cbs)
            {
                i.second(m_val, val);
            }
        }
        auto lk_guard = LockGuard<MutexType, WriteLockTag>(m_mutex);
        m_val         = val;
    }

    auto getTypeName() const
        -> std::string_view
    {
        return Sylar::typeToName<T>();
    }

    /**
     * @brief 添加变化回调函数
     * @return 返回该回调函数对应的唯一id,用于删除回调
     */
    // auto addListen

    ~ConfigVar() = default;

private:
    mutable MutexType m_mutex;
    T m_val;

    //变更回调函数组, uint64_t key,要求唯一，一般可以用hash
    std::map<uint64_t, OnChangeCb> m_cbs;
};
} //namespace Cot
#endif