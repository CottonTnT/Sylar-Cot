#include "sylarweb/logevent.h"
#include "sylarweb/log.h"
namespace Cot {
/* ======================== LogEvent ======================== */
LogEvent::LogEvent(const std::string& logger_name,
                   LogLevel::Level level,
                   const char* file,
                   int32_t line,
                   uint32_t elapse,
                   uint32_t thread_id,
                   uint32_t fiber_id,
                   uint64_t time,
                   const std::string& thread_name)
    : m_loggername(logger_name)
    , m_level(level)
    , m_filename(file)
    , m_line(line)
    , m_elapse(elapse)
    , m_threadid(thread_id)
    , m_fiberid(fiber_id)
    , m_time(time)
    , m_threadname(thread_name)

{
}

void LogEvent::format(const char* fmt, ...)
{
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al)
{
    char* buf = nullptr;
    int len   = vasprintf(&buf, fmt, al);
    if (len != -1)
    {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

/* ======================== LogEventWrap ======================== */

LogEventWrap::LogEventWrap(Logger::Sptr logger, LogEvent::Sptr event)
    : m_logger(logger)
    , m_event(event)
{
}

LogEventWrap::~LogEventWrap()
{
    m_logger->log(m_event);
}

} //namespace Cot