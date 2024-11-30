#ifndef COT_CONFIG_H
#define COT_CONFIG_H

#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include "log.h"
#include <boost/lexical_cast.hpp>

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

    virtual ~ConfigVarBase() { }

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

template <typename From, typename To>
class LexicalCast {
public:
    To operator()(const From&& v)
    {
        return boost::lexical_cast<To>(v);
    }
};

template <typename T>
class LexicalCast<std::string, std::vector<T>> {
public:
    auto operator()(const std::string& v)
        -> std::vector<T>
    {
    }
};

#endif