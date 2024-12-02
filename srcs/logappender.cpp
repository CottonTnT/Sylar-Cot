#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include "sylarweb/logevent.h"
#include "sylarweb/logappender.h"
#include "sylarweb/logformatter.h"

namespace Cot {

/**
 * @brief 日志输出器，用于输出一个日志事件。这是一个虚类，可以派生出不同的具体实现，比如往输出到终端的StdoutLogAppender，以及输出到文件的FileLogAppender。
 */
/* ======================== LogAppender ======================== */
LogAppenderBase::LogAppenderBase(LogFormatter::Sptr default_formatter)
    : m_formatter(default_formatter)
{
}

void LogAppenderBase::setFormatter(LogFormatter::Sptr formatter)
{
    auto lk_guard = std::lock_guard<MutexType>(m_mutex);
    m_formatter   = formatter;
}

auto LogAppenderBase::getFormatter() const
    -> LogFormatter::Sptr
{
    auto lk_guard = std::lock_guard<MutexType>(m_mutex);
    return m_formatter;
}

/* ======================== FileAppender ======================== */

FileLogAppender::FileLogAppender(std::string_view filename)
    : m_filename(filename)
{
    m_filename = filename;
    reopen();
}

void FileLogAppender::log(LogFormatter::Sptr fmter, LogEvent::Sptr event)
{
    m_filestream << fmter->format(event);
}

bool FileLogAppender::reopen()
{
    if (m_filestream)
    {
        m_filestream.close();
    }
    m_filestream.open(m_filename);

    return not not m_filestream;
}

/* ======================== StdoutLogAppender ======================== */
void StdoutLogAppender::log(LogFormatter::Sptr fmter, LogEvent::Sptr event)
{
    std::cout << fmter->format(event);
}

} //namespace Cot