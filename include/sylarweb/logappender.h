#ifndef LOGAPPENDER_H
#define LOGAPPENDER_H

#include <memory>
#include "mutex.h"
#include <fstream>
#include "sylarweb/logformatter.h"

namespace Cot {
class LogFormatter;
class LogEvent;

class LogAppenderBase {
public:
    using Sptr      = std::shared_ptr<LogAppenderBase>;
    using MutexType = SpinMutex;

    LogAppenderBase(std::shared_ptr<LogFormatter> default_formatter = std::make_shared<LogFormatter>());

    virtual void log(std::shared_ptr<LogEvent> event) = 0;

    auto getFormatter() const
        -> std::shared_ptr<LogFormatter>;

    auto setFormatter(std::shared_ptr<LogFormatter> formatter)
        -> void;

    virtual auto toYamlString()
        -> std::string = 0;

    virtual ~LogAppenderBase() = default;

protected:
    mutable MutexType m_mutex;
    std::shared_ptr<LogFormatter> m_formatter;
};

template <typename Appender>
class LogAppenderImpl : public LogAppenderBase {
public:
    LogAppenderImpl() = default;

    template <typename... Ts>
    LogAppenderImpl(std::shared_ptr<LogFormatter> formatter, Ts... ts)
        : LogAppenderBase(formatter)
        , m_appender(std::forward<Ts>(ts)...)
    {
    }

    void log(std::shared_ptr<LogEvent> event) override
    {
        m_appender.log(m_formatter, event);
    }

    auto toYamlString() -> std::string override
    {
        return "";
    }

private:
    Appender m_appender;
};

class StdoutLogAppender {
public:
    using Sptr = std::shared_ptr<StdoutLogAppender>;

    void log(std::shared_ptr<LogFormatter> fmter, std::shared_ptr<LogEvent> event);

    auto toYamlString()
        -> std::string;
};

class FileLogAppender {
public:
    using Sptr = std::shared_ptr<FileLogAppender>;

    FileLogAppender(std::string_view filename);

    void log(std::shared_ptr<LogFormatter> fmter, std::shared_ptr<LogEvent> event);
    auto toYamlString()
        -> std::string;

    bool reopen();

private:
    //文件路径
    std::string m_filename;
    std::ofstream m_filestream;
    //上次打开的时间
    uint64_t m_lastTime = 0;
    // 文件打开错误标识
    bool m_reopenError = false;
};
#endif
} //namespace Cot