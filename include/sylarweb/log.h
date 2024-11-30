#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>

#include <stdint.h>
#include <vector>
#include <map>

#include "util.h"
#include "singleton.hpp"
#include "mutex.h"
#include "loglevel.h"

#define COT_LOG_LEVEL(logger, level)                                                                                                                                            \
    do {                                                                                                                                                                        \
        if (logger->getLevel() <= level)                                                                                                                                        \
        {                                                                                                                                                                       \
            Cot::LogEventWrap(logger, Cot::LogEvent::Sptr(new Cot::LogEvent("nihao", level, __FILE__, __LINE__, 0, Sylar::getThreadId(), Sylar::getFiberId(), time(0), "hhh"))) \
                .getLogEvent()                                                                                                                                                  \
                ->getSS();                                                                                                                                                      \
        }                                                                                                                                                                       \
    } while (0)

#define COT_LOG_DEBUG(logger) COT_LOG_LEVEL(logger, Cot::LogLevel::DEBUG)

#define COT_LOG_INFO(logger)  COT_LOG_LEVEL(logger, Cot::LogLevel::INFO)
#define COT_LOG_WARN(logger)  COT_LOG_LEVEL(logger, Cot::LogLevel::WARN)
#define COT_LOG_ERROR(logger) COT_LOG_LEVEL(logger, Cot::LogLevel::ERROR)
#define COT_LOG_FATAL(logger) COT_LOG_LEVEL(logger, Cot::LogLevel::FATAL)

namespace Cot {

class Logger;
class LoggerManager;
class LogEvent;
class LogAppenderBase;

/**
 * @brief 日志器，用于输出日志。这个类是直接与用户进行交互的类，提供log方法用于输出日志事件。不带root logger

Logger的实现包含了日志级别，日志器名称，创建时间，以及一个LogAppender数组，日志事件由log方法输出，log方法首先判断日志级别是否达到本Logger的级别要求，是则将日志传给各个LogAppender进行输出，否则抛弃这条日志。
 */
class Logger : public std::enable_shared_from_this<Logger> {
public:
    using Sptr      = std::shared_ptr<Logger>;
    using MutexType = SpinMutex;

    Logger(std::string_view name = "default");

    void log(std::shared_ptr<LogEvent> event);

    void addAppender(std::shared_ptr<LogAppenderBase> appender);
    void delAppender(std::shared_ptr<LogAppenderBase> appender);
    void clearAppender();

    auto getName() const
        -> std::string_view { return m_name; }

    auto getLevel() const
        -> LogLevel::Level { return m_level; }

    auto setLevel(LogLevel::Level level)
        -> void { m_level = level; }

private:
    MutexType m_mutex;
    std::string m_name;                                      //日志器名称
    LogLevel::Level m_level;                                 //日志级别
    std::list<std::shared_ptr<LogAppenderBase>> m_appenders; //Appender集合
    uint64_t m_createTime;                                   //创建时间
};

} //namespace Cot

#endif