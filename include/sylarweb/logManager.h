#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <unordered_map>
#include "singleton.hpp"
#include "mutex.h"

namespace Cot {
/**
 * @brief 日志器管理类
 */

class Logger;
class LoggerManager {
public:
    typedef SpinMutex MutexType;
    /**
     * @brief 构造函数
     */
    LoggerManager();

    /**
     * @brief 获取日志器
     * @param[in] name 日志器名称
     */
    auto getLogger(const std::string& name)
        -> std::shared_ptr<Logger>;

    /**
     * @brief 初始化
     */
    void init();

    /**
     * @brief 返回主日志器
     */
    auto getRoot()
        -> std::shared_ptr<Logger> const { return m_root; }

    /**
     * @brief 将所有的日志器配置转成YAML String
     */
    std::string toYamlString();

private:
    /// Mutex
    MutexType m_mutex;
    /// 日志器容器
    std::unordered_map<std::string, std::shared_ptr<Logger>> m_loggers;
    /// 主日志器
    std::shared_ptr<Logger> m_root;
};

/// 日志器管理类单例模式
typedef Cot::Singleton<LoggerManager> LoggerMgr;
} //namespace Cot
#endif