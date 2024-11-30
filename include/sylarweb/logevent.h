#ifndef COT_LOGEVENT_H
#define COT_LOGEVENT_H
#include <string>
#include <sstream>
#include "loglevel.h"
#include <memory>
#include <stdarg.h>

namespace Cot {
/**
 * @brief 日志事件，用于记录日志现场，比如该日志的级别，文件名/行号，日志消息，线程/协程号，所属日志器名称等。
 */

class LogEvent {
public:
    using Sptr = std::shared_ptr<LogEvent>;
    /**
     * @brief 构造函数
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] filename 文件名
     * @param[in] line 文件行号
     * @param[in] elapse 程序启动依赖的耗时(毫秒)
     * @param[in] thread_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time 日志事件(UTC秒)
     * @param[in] thread_name 线程名称
     */
    LogEvent(const std::string& logger_name,
             LogLevel::Level level,
             const char* filename,
             int32_t line,
             uint32_t elapse,
             uint32_t thread_id,
             uint32_t fiber_id,
             uint64_t time,
             const std::string& thread_name);

    auto getFile() const
        -> const char* { return m_filename; }

    auto getLine() const
        -> int32_t { return m_line; }
    auto getElapse() const
        -> uint32_t { return m_elapse; }
    auto getThreadId() const
        -> uint32_t { return m_threadid; }
    auto getFiberId() const
        -> uint32_t { return m_fiberid; }
    auto getTime() const
        -> uint64_t { return m_time; }

    auto getThreadName() const
        -> std::string_view { return m_threadname; }
    auto getLoggerName() const
        -> std::string_view { return m_loggername; }

    auto getContent() const
        -> std::string { return m_ss.str(); }

    auto getSS()
        -> std::stringstream& { return m_ss; }

    auto getLevel() const
        -> LogLevel::Level { return m_level; }

    void format(const char* fmt, ...);

    void format(const char* fmt, va_list al);

private:
    std::string m_loggername;
    LogLevel::Level m_level;

    /// 文件名
    const char* m_filename = nullptr;
    /// 行号
    int32_t m_line = 0;
    /// 程序启动开始到现在的毫秒数
    uint32_t m_elapse = 0;
    /// 线程ID
    uint32_t m_threadid = 0;
    /// 协程ID
    uint32_t m_fiberid = 0;
    /// 时间戳
    uint64_t m_time = 0;
    std::string m_threadname;
    std::stringstream m_ss; /// 日志内容流
};

class Logger;

class LogEventWrap {
public:
    LogEventWrap(std::shared_ptr<Logger> logger, LogEvent::Sptr event);
    ~LogEventWrap();
    auto getLogEvent() const { return m_event; }

private:
    std::shared_ptr<Logger> m_logger;
    LogEvent::Sptr m_event;
};

}

#endif